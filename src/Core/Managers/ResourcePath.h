#pragma once

#include "TextUtils.h"

#include <string>


namespace RESOURCE_PATH
{
	const std::string ROOT		= PATH_JOIN("res");
	const std::string MODELS	= PATH_JOIN(ROOT, "Models");
	const std::string TEXTURES	= PATH_JOIN(ROOT, "Textures");
	const std::string SHADERS	= PATH_JOIN(ROOT, "Shaders");
}

namespace SOURCE_PATH
{
	const std::string ROOT		= PATH_JOIN("src");
}
