#include "Scene.h"

#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <optix.h>

#include "GL/freeglut.h"

#include "Mie.h"
#include "../sutil/sutil.h"

Scene::Scene(uint32_t width, uint32_t height, float sampleStep) :
    width(width), height(height), sampleStep(sampleStep), opticalDensityMultiplier(sampleStep * 2048)
{
    context = optix::Context::create();

    context["lightDirection"]->setFloat(0.5f, -0.5f, -0.5f);
    context["lightColor"]->setFloat(1.2f, 1.0f, 0.7f);
    context["lightIntensity"]->setFloat(1e9);


    //context["skyIntensity"]->setFloat(0, 0, 0);
    //context["groundIntensity"]->setFloat(0, 0, 0);
    context["skyIntensity"]->setFloat(900, 1000, 1600);
    context["groundIntensity"]->setFloat(600, 300, 200);

    context->setRayTypeCount(1);
    context->setEntryPointCount(1);

    progressiveBuffer = context->createBuffer(RT_BUFFER_OUTPUT, RT_FORMAT_FLOAT4, width, height);
    screenBuffer = context->createBuffer(RT_BUFFER_OUTPUT, RT_FORMAT_UNSIGNED_BYTE4, width, height);
}


Scene::~Scene()
{
}


void Scene::addCloud(const std::string &path)
{
    cloudBuffer = loadVolumetricData(path);

    cloudSampler = createSamplerForBuffer3D(cloudBuffer);
    context["cloud"]->setTextureSampler(cloudSampler);

    RTsize cloudX, cloudY, cloudZ;
    cloudBuffer->getSize(cloudX, cloudY, cloudZ);

    optix::Buffer inScatterOut = context->createBuffer(RT_BUFFER_OUTPUT, RT_FORMAT_FLOAT, cloudX, cloudY, cloudZ);

    optix::Program inScatter = loadProgram("inScatter.cu", "inScatter");
    inScatter["resultBuffer"]->setBuffer(inScatterOut);
    context->setRayGenerationProgram(0, inScatter);

    context->validate();
    context->launch(0, cloudX, cloudY, cloudZ);

    cloudSampler->destroy();
    cloudBuffer->destroy();

    inScatterIn = context->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT, cloudX, cloudY, cloudZ);
    float_t* to = (float_t*)inScatterIn->map();
    float_t* from = (float_t*)inScatterOut->map();
    std::memcpy(to, from, sizeof(float_t) * cloudX * cloudY * cloudZ);
    inScatterIn->unmap();
    inScatterOut->unmap();

    inScatterOut->destroy();
    cloudBuffer = loadVolumetricData(path);
    cloudSampler = createSamplerForBuffer3D(cloudBuffer);
    context["cloud"]->setTextureSampler(cloudSampler);

    inScatterSampler = createSamplerForBuffer3D(inScatterIn);
    context["inScatter"]->setTextureSampler(inScatterSampler);


    cloudGeometry = context->createGeometry();
    cloudGeometry->setBoundingBoxProgram(loadProgram("cloud.cu", "bounds"));
    cloudGeometry->setIntersectionProgram(loadProgram("cloud.cu", "intersect"));
    cloudGeometry->setPrimitiveCount(1u);
    cloudGeometry["minimalRayDistance"]->setFloat(0.001f);

    cloudMaterial = context->createMaterial();
    cloudMaterial->setClosestHitProgram(0, loadProgram("cloud.cu", "closestHitRadiance"));

    cloudInstance = context->createGeometryInstance(cloudGeometry, &cloudMaterial, &cloudMaterial + 1);
    cloudGroup = context->createGeometryGroup();
    cloudGroup->addChild(cloudInstance);
    cloudGroup->setAcceleration(context->createAcceleration("MedianBvh", "Bvh"));

    context["objectRoot"]->set(cloudGroup);

    context->setRayGenerationProgram(0, camera);
    context["radianceRayType"]->setUint(0u);
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
    context["opticalDensityMultiplier"]->setFloat(opticalDensityMultiplier);

    context["mie"]->setTextureSampler(Mie::getMieSampler(context));
    context["choppedMie"]->setTextureSampler(Mie::getChoppedMieSampler(context));
    context["choppedMieIntegral"]->setTextureSampler(Mie::getChoppedMieIntegralSampler(context));

    context["progressiveBuffer"]->setBuffer(progressiveBuffer);

    camera = loadProgram("pinholeCamera.cu", "pinholeCamera");

    clearScreen = loadProgram("pinholeCamera.cu", "pinholeCamera");

    cameraEye = optix::make_float3(0, 0, -0.7f);
    cameraLookat = optix::make_float3(0, -0.3f, 0);
    cameraUp = optix::make_float3(0, 1, 0);

    cameraRotate = optix::Matrix4x4::identity();
    updateCamera();

    exception = loadProgram("pinholeCamera.cu", "exception");
    context->setExceptionProgram(0, exception);
    context["errorColor"]->setFloat(1, 0.6f, 0.6f);

    miss = loadProgram("pinholeCamera.cu", "miss");
    context->setMissProgram(0, miss);
    context["missColor"]->setFloat(900, 1000, 1600);

    reinhardFirstPass = loadProgram("reinhard.cu", "firstPass");
    reinhardSecondPass = loadProgram("reinhard.cu", "secondPass");
    reinhardLastPass = loadProgram("reinhard.cu", "applyReinhard");

    context["sumLogColumns"]->setBuffer(context->createBuffer(RT_BUFFER_INPUT_OUTPUT | RT_BUFFER_GPU_LOCAL, RT_FORMAT_FLOAT, width));
    context["lAverage"]->setBuffer(context->createBuffer(RT_BUFFER_INPUT_OUTPUT | RT_BUFFER_GPU_LOCAL, RT_FORMAT_FLOAT, 1));
    context["totalPixels"]->setUint(width * height);
    context["screenBuffer"]->setBuffer(screenBuffer);
}

void Scene::rotateCamera(optix::float2 from, optix::float2 to)
{
    cameraRotate = arcball.rotate(from, to);
    updateCamera();
    restartProgressive();
}

void Scene::updateCamera()
{
    const float vfov = 90.0f;
    const float aspectRatio = static_cast<float>(width) /
        static_cast<float>(height);

    optix::float3 u, v, w;
    sutil::calculateCameraVariables(
        cameraEye, cameraLookat, cameraUp, vfov, aspectRatio,
        u, v, w);

    const optix::Matrix4x4 frame = optix::Matrix4x4::fromBasis(
        normalize(u),
        normalize(v),
        normalize(w),
        cameraLookat);
    const optix::Matrix4x4 transform = frame * cameraRotate * frame.inverse();

    cameraEye = make_float3(transform * make_float4(cameraEye, 1.0f));

    sutil::calculateCameraVariables(
        cameraEye, cameraLookat, cameraUp, vfov, aspectRatio,
        u, v, w, true);

    cameraRotate = optix::Matrix4x4::identity();

    camera["eye"]->setFloat(cameraEye);
    camera["U"]->setFloat(u);
    camera["V"]->setFloat(v);
    camera["W"]->setFloat(w);
}


void Scene::display()
{
    context->validate();

    subframeId++;
    context["subframeId"]->setUint(subframeId);
    context->setRayGenerationProgram(0, camera);
    context->launch(0, width, height);

    context->setRayGenerationProgram(0, reinhardFirstPass);
    context->launch(0, width);

    context->setRayGenerationProgram(0, reinhardSecondPass);
    context->launch(0, 1);

    reinhardLastPass["midGrey"]->setFloat(midGrey);
    context->setRayGenerationProgram(0, reinhardLastPass);
    context->launch(0, width, height);

    GLenum glDataType = GL_UNSIGNED_BYTE;
    GLenum glFormat = GL_RGBA;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
    GLvoid* imageData = screenBuffer->map();
    glDrawPixels(width, height, glFormat, glDataType, imageData);
    screenBuffer->unmap();
    
    //sutil::displayBufferGL(progressiveBuffer);
}

void Scene::increaseExposure()
{
    midGrey *= 1.2f;
}

void Scene::decreaseExposure()
{
    midGrey /= 1.2f;
}

optix::Buffer Scene::loadVolumetricData(const std::string &path, bool border)
{
    std::ifstream fin(path, std::ios::binary);
    uint32_t sizeX, sizeY, sizeZ;
    fin.read(reinterpret_cast<char*>(&sizeX), sizeof(uint32_t));
    fin.read(reinterpret_cast<char*>(&sizeY), sizeof(uint32_t));
    fin.read(reinterpret_cast<char*>(&sizeZ), sizeof(uint32_t));

    uint32_t maxSize = std::max({ sizeX, sizeY, sizeZ });
    uint32_t sourceSize = sizeX * sizeY * sizeZ;
    uint32_t targetSize = maxSize * maxSize * maxSize;

    std::cout << "Creating buffer" << std::endl;

    optix::Buffer buffer = context->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_FLOAT);
    buffer->setSize(maxSize, maxSize, maxSize);

    {
        std::unique_ptr<float_t[]> sourceDensity(new float_t[sourceSize]);
        std::cout << "Trying to read volumetric data of " << sizeX << "x" << sizeY << "x" << sizeZ << std::endl;
        fin.read(reinterpret_cast<char*>(&sourceDensity[0]), sizeof(float_t) * sourceSize);
        std::cout << "Read has completed successfuly" << std::endl;

        float_t* density = (float_t*)buffer->map();

        std::memset(density, 0, sizeof(float_t) * maxSize);

        for (uint32_t z = 0; z < sizeZ; z++)
        {
            for (uint32_t y = 0; y < sizeY; y++)
            {
                for (uint32_t x = 0; x < sizeX; x++)
                {
                    uint32_t targetPos = x + maxSize * y + maxSize * maxSize * z;
                    uint32_t sourcePos = sizeX * sizeY * z + sizeX * y + x;

                    bool isFace = false;
                    isFace |= x == 0 || x == maxSize - 1;
                    isFace |= y == 0 || y == maxSize - 1;
                    isFace |= z == 0 || z == maxSize - 1;

                    if (!isFace)
                    {
                        /*
                        Converting physical density to optical density. This allows to make calculations independent from step size.
                        An optical density can be derived from a real density in the following way:
                        opticalDensity = -log(1 - density)

                        Let's prove it.

                        assert(transmittance == 1 - absorbed)

                        After one sample of a volume absorbance is changed as follows:
                        absorbed' = absorbed + density * (1 - absorbed) 
                                  = absorbed * (1 - density) + density

                        And transmittance:
                        transmittance' = transmittance * e^-opticalDensity

                        Now Let's derive equation for absorbance from transmittance.
                        1 - absorbed' = (1 - absorbed) * e^-opticalDensity 
                                      = e^-opticalDensity - absorbed * e^-opticalDensity

                        And therefore
                        absorbed' = absorbed * e^-opticalDensity - e^-opticalDensity + 1 
                                  = absorbed * (1 - density) + density

                        Q.E.D.
                        */
                        
                        // Additionaly scale the source density to range from 0 to 1,
                        density[targetPos] = -logf(1 - sourceDensity[sourcePos] / 30.0f);
                    }
                }
            }
        }

        buffer->unmap();
    }

    return buffer;
}

optix::TextureSampler Scene::createSamplerForBuffer3D(optix::Buffer buffer)
{
    optix::TextureSampler sampler3D = context->createTextureSampler();

    for (uint32_t dim = 0; dim < 3; dim++)
    {
        sampler3D->setWrapMode(dim, RT_WRAP_CLAMP_TO_EDGE);
    }

    sampler3D->setFilteringModes(
        RT_FILTER_LINEAR,
        RT_FILTER_LINEAR,
        RT_FILTER_NONE
    );

    sampler3D->setIndexingMode(RT_TEXTURE_INDEX_NORMALIZED_COORDINATES);
    sampler3D->setBuffer(buffer);

    return sampler3D;
}

optix::Program Scene::loadProgram(const std::string &fileName, const std::string programName)
{
    const std::string ptxPath = "./CUDA/";
    const std::string ptxExtension = ".ptx";
    
    std::string path = ptxPath + fileName + ptxExtension;

    std::cout << "loading program " << path;

    return context->createProgramFromPTXFile(path, programName);
}