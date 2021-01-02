#pragma once

#include "core/gpu/mesh.h"

#include "utils/glm_utils.h"

#include <string>


namespace object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

}
