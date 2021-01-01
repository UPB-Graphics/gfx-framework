#pragma once

/*
 *	Graphic Engine
 */

#include "include/gl.h"
#include "include/glm_utils.h"
#include "include/math_utils.h"
#include "include/memory_utils.h"

#include "core/gpu/gpu_buffers.h"
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "core/gpu/texture2D.h"

#include "core/world.h"

#include "components/camera.h"

#include "core/managers/resource_path.h"
#include "core/managers/texture_manager.h"

#include "core/window/window_object.h"
#include "core/window/input_controller.h"


class Engine
{
	public:
		static WindowObject* Init(WindowProperties &props);

		static WindowObject* GetWindow();

		// Get elapsed time in seconds since the application started
		static double GetElapsedTime();

		static void Exit();

	private:
		static WindowObject* window;
};
