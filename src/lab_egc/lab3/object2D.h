#pragma once

#include "include/glm_utils.h"
#include "core/gpu/mesh.h"

#include <string>


namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

}
