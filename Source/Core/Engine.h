#pragma once

/*
 *	Graphic Engine
 */

#include <include/gl.h>
#include <include/glm.h>
#include <include/math.h>
#include <include/utils.h>

#include <Core/GPU/GPUBuffers.h>
#include <Core/GPU/Mesh.h>
#include <Core/GPU/Shader.h>
#include <Core/GPU/Texture2D.h>

#include <Core/World.h>

#include <Component/Camera/Camera.h>

#include <Core/Managers/ResourcePath.h>
#include <Core/Managers/TextureManager.h>

#include <Core/Window/WindowObject.h>
#include <Core/Window/InputController.h>

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