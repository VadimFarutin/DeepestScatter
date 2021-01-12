

#include "HDRCubeLoader.h"
#include "HDRLoader.h"

#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

//-----------------------------------------------------------------------------
//  
//  Utility functions 
//
//-----------------------------------------------------------------------------

optix::TextureSampler loadHDRCubeBuffer(optix::Context context,
    const std::string& filename)
{
    // Create tex sampler and populate with default values
    optix::TextureSampler sampler = context->createTextureSampler();
    sampler->setWrapMode(0, RT_WRAP_CLAMP_TO_EDGE);
    sampler->setWrapMode(1, RT_WRAP_CLAMP_TO_EDGE);
    sampler->setWrapMode(2, RT_WRAP_CLAMP_TO_EDGE);
    sampler->setIndexingMode(RT_TEXTURE_INDEX_NORMALIZED_COORDINATES);
    sampler->setReadMode(RT_TEXTURE_READ_NORMALIZED_FLOAT);
    sampler->setMaxAnisotropy(1.0f);
    sampler->setMipLevelCount(1u);
    sampler->setArraySize(1u);

    optix::Buffer buffer = context->createBuffer(RT_BUFFER_INPUT | RT_BUFFER_CUBEMAP, RT_FORMAT_FLOAT4);
    float* buffer_data = 0;
    unsigned int cur_face_offset = 0;
    const size_t faces_num = 6;

    // Read in HDR, set texture buffer to empty buffer if fails
    HDRLoader hdr(filename);
    if (!hdr.failed()) {

        const unsigned int nx = hdr.width() / faces_num;
        const unsigned int ny = hdr.height();

        for (size_t face = 0; face < faces_num; ++face) {
            if (face == 0) {
                buffer->setSize(nx, ny, faces_num);
                buffer_data = static_cast<float*>(buffer->map());
                cur_face_offset = 0;
            }
            else {
                buffer_data += nx * ny * 4;
                cur_face_offset += nx;
            }

            for (unsigned int i = 0; i < nx; ++i) {
                for (unsigned int j = 0; j < ny; ++j) {
                    unsigned int hdr_index = ((j)*nx * faces_num + cur_face_offset + i) * 4;
                    unsigned int buf_index = ((j)*nx + i) * 4;

                    //buffer_data[buf_index + 0] = hdr.raster()[hdr_index + 0];
                    //buffer_data[buf_index + 1] = hdr.raster()[hdr_index + 1];
                    //buffer_data[buf_index + 2] = hdr.raster()[hdr_index + 2];
                    //buffer_data[buf_index + 3] = hdr.raster()[hdr_index + 3];

                    buffer_data[buf_index + 0] = hdr.raster()[hdr_index + 0];
                    buffer_data[buf_index + 1] = hdr.raster()[hdr_index + 1];
                    buffer_data[buf_index + 2] = hdr.raster()[hdr_index + 2];
                    buffer_data[buf_index + 3] = 1.0;

                    //buffer_data[buf_index + 0] = hdr.raster()[hdr_index + 0] * 1000.0;
                    //buffer_data[buf_index + 1] = 0.0;
                    //buffer_data[buf_index + 2] = 0.0;
                    //buffer_data[buf_index + 3] = hdr.raster()[hdr_index + 3];
                }
            }

        }

        buffer->unmap();
    }

    sampler->setBuffer(0u, 0u, buffer);
    sampler->setFilteringModes(RT_FILTER_LINEAR, RT_FILTER_LINEAR, RT_FILTER_NONE);

    return sampler;
}
