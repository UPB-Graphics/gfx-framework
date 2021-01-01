#pragma once

#include <include/glm.h>

namespace Transform3D
{
	// Translate matrix
	inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
	{
		// TODO implement translate matrix
		return glm::mat4(1);
	}

	// Scale matrix
	inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
	{
		// TODO implement scale matrix
		return glm::mat4(1);
	}

	// Rotate matrix relative to the OZ axis
	inline glm::mat4 RotateOZ(float radians)
	{
		// TODO implement rotate matrix
		return glm::mat4(1);
	}

	// Rotate matrix relative to the OY axis
	inline glm::mat4 RotateOY(float radians)
	{
		// TODO implement rotate matrix
		return glm::mat4(1);
	}

	// Rotate matrix relative to the OX axis
	inline glm::mat4 RotateOX(float radians)
	{
		// TODO implement rotate matrix
		return glm::mat4(1);
	}
}
