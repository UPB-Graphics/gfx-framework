#pragma once

#include <string>

#include "utils/text_utils.h"


namespace RESOURCE_PATH
{
    const std::string ROOT      = PATH_JOIN("res");
    const std::string MODELS    = PATH_JOIN(ROOT, "models");
    const std::string TEXTURES  = PATH_JOIN(ROOT, "textures");
    const std::string SHADERS   = PATH_JOIN(ROOT, "shaders");
}

namespace SOURCE_PATH
{
    const std::string EGC       = PATH_JOIN("src", "lab_egc");
    const std::string SPG       = PATH_JOIN("src", "lab_spg");
    const std::string EXTRA     = PATH_JOIN("src", "lab_extra");
}
