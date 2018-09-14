#include "Scene.h"

#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>

#include <gsl/gsl_util>

#include <optix.h>
#include "GL/freeglut.h"

#pragma warning(push, 0)
#include "Util/sutil.h"

#include <openvdb/openvdb.h>
#include <openvdb/io/Stream.h>

#include <openvdb/Types.h>
#include <openvdb/tools/Statistics.h>
#include <openvdb/math/Stats.h>
#pragma warning(pop)

#include "SceneItem.h"
#include "Util/Resources.h"
#include "Mie.h"

namespace DeepestScatter
{
    Scene::Scene(const Settings& settings, const std::vector<std::shared_ptr<SceneItem>>& sceneItems, optix::Context context,
        std::shared_ptr<Resources>& resources):
        width(settings.width), height(settings.height), sampleStep(settings.sampleStep), 
        sceneItems(sceneItems),
        context(context), resources(resources)
    {
        auto lightDirection = optix::normalize(optix::make_float3(-0.586f, -0.766f, -0.2717f));
        context["lightDirection"]->setFloat(lightDirection);
        context["lightColor"]->setFloat(1.3f, 1.25f, 1.15f);
        context["lightIntensity"]->setFloat(6e5f);

        context["skyIntensity"]->setFloat(.6f, .6f, 2);
        context["groundIntensity"]->setFloat(.6f, .8f, 1);
        //context["skyIntensity"]->setFloat(0, 0, 0);
        //context["groundIntensity"]->setFloat(0, 0, 0);

        context->setRayTypeCount(1);
        context->setEntryPointCount(1);

        progressiveBuffer = context->createBuffer(RT_BUFFER_INPUT_OUTPUT, RT_FORMAT_FLOAT4, width, height);
        varianceBuffer = context->createBuffer(RT_BUFFER_INPUT_OUTPUT, RT_FORMAT_FLOAT4, width, height);
        screenBuffer = context->createBuffer(RT_BUFFER_OUTPUT, RT_FORMAT_UNSIGNED_BYTE4, width, height);
    }

    void Scene::addCloud(const std::string &path)
    {
    }

    void Scene::restartProgressive()
    {
        std::cout << "restarting progressive" << std::endl;
        subframeId = 0;
        context->setRayGenerationProgram(0, clearScreen);
        context->launch(0, width, height);
    }

    void Scene::init()
    {
        context["sampleStep"]->setFloat(sampleStep);
        context["densityMultiplier"]->setFloat(cloudLengthMeters / meanFreePathMeters);

        context["mie"]->setTextureSampler(Mie::getMieSampler(context));
        context["choppedMie"]->setTextureSampler(Mie::getChoppedMieSampler(context));
        context["choppedMieIntegral"]->setTextureSampler(Mie::getChoppedMieIntegralSampler(context));

        context["progressiveBuffer"]->setBuffer(progressiveBuffer);
        context["varianceBuffer"]->setBuffer(varianceBuffer);

        camera = resources->loadProgram("pinholeCamera.cu", "pinholeCamera");

        clearScreen = resources->loadProgram("pinholeCamera.cu", "clearScreen");

        cameraEye = optix::make_float3(2, -0.4f, 0);
        cameraLookat = optix::make_float3(0, 0, 0);
        cameraUp = optix::make_float3(0, 1, 0);

        cameraRotate = optix::Matrix4x4::identity();
        updateCamera();

        exception = resources->loadProgram("pinholeCamera.cu", "exception");
        context->setExceptionProgram(0, exception);
        context["errorColor"]->setFloat(1, 0.6f, 0.6f);

        miss = resources->loadProgram("pinholeCamera.cu", "miss");
        context->setMissProgram(0, miss);
        context["missColor"]->setFloat(900, 1000, 1600);

        reinhardFirstPass = resources->loadProgram("reinhard.cu", "firstPass");
        reinhardSecondPass = resources->loadProgram("reinhard.cu", "secondPass");
        reinhardLastPass = resources->loadProgram("reinhard.cu", "applyReinhard");

        context["sumLogColumns"]->setBuffer(context->createBuffer(RT_BUFFER_INPUT_OUTPUT | RT_BUFFER_GPU_LOCAL, RT_FORMAT_FLOAT, width));
        context["lAverage"]->setBuffer(context->createBuffer(RT_BUFFER_INPUT_OUTPUT | RT_BUFFER_GPU_LOCAL, RT_FORMAT_FLOAT, 1));
        context["totalPixels"]->setUint(width * height);
        context["screenBuffer"]->setBuffer(screenBuffer);

        for each (auto item in sceneItems)
        {
            item->Init();
        }

        context->setRayGenerationProgram(0, camera);
        context["radianceRayType"]->setUint(0u);
    }

    void Scene::rotateCamera(optix::float2 from, optix::float2 to)
    {
        cameraRotate = arcball.rotate(from, to);
        updateCamera();
        restartProgressive();
    }

    void Scene::updateCamera()
    {
        const float hfov = 30.0f;
        const float aspectRatio = static_cast<float>(width) /
            static_cast<float>(height);

        optix::float3 u, v, w;
        sutil::calculateCameraVariables(
            cameraEye, cameraLookat, cameraUp, hfov, aspectRatio,
            u, v, w, false);

        const optix::Matrix4x4 frame = optix::Matrix4x4::fromBasis(
            normalize(u),
            normalize(v),
            normalize(w),
            cameraLookat);
        const optix::Matrix4x4 transform = frame * cameraRotate * frame.inverse();

        cameraEye = make_float3(transform * make_float4(cameraEye, 1.0f));

        sutil::calculateCameraVariables(
            cameraEye, cameraLookat, cameraUp, hfov, aspectRatio,
            u, v, w, false);

        cameraRotate = optix::Matrix4x4::identity();

        camera["eye"]->setFloat(cameraEye);
        camera["U"]->setFloat(u);
        camera["V"]->setFloat(v);
        camera["W"]->setFloat(w);
    }


    void Scene::display()
    {
        context->validate();

        for (int i = 0; i < 10; i++)
        {
            subframeId++;
            context["subframeId"]->setUint(subframeId);
            context->setRayGenerationProgram(0, camera);
            context->launch(0, width, height);

            context->setRayGenerationProgram(0, reinhardFirstPass);
            context->launch(0, width, 1);

            context->setRayGenerationProgram(0, reinhardSecondPass);
            context->launch(0, 1, 1);

            context["exposure"]->setFloat(exposure);
            context->setRayGenerationProgram(0, reinhardLastPass);
            context->launch(0, width, height);
        }

        GLenum glDataType = GL_UNSIGNED_BYTE;
        GLenum glFormat = GL_RGBA;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        GLvoid* imageData = screenBuffer->map();
        glDrawPixels(width, height, glFormat, glDataType, imageData);
        screenBuffer->unmap();
    }

    void Scene::increaseExposure()
    {
        exposure *= 1.2f;
    }

    void Scene::decreaseExposure()
    {
        exposure /= 1.2f;
    }
}