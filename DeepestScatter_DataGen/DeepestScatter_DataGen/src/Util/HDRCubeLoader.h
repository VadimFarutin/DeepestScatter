#pragma once

#include <optixu/optixpp_namespace.h>
#include "sutil.h"
#include <string>
#include <iosfwd>

 //-----------------------------------------------------------------------------
 //
 // Utility functions
 //
 //-----------------------------------------------------------------------------

// Creates a Buffer object for the given cubemap files.
SUTILAPI optix::TextureSampler loadHDRCubeBuffer(optix::Context context,
    const std::string& filename);
