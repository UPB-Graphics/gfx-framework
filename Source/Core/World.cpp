#include "World.h"

#include <Core/Engine.h>
#include <Component/CameraInput.h>
#include <Component/Transform/Transform.h>

World::World()
{
	previousTime = 0;
	elapsedTime = 0;
	deltaTime = 0;
	paused = false;
	shouldClose = false;

	window = Engine::GetWindow();
}

void World::Run()
{
	if (!window)
		return;

	while (!window->ShouldClose())
	{
		LoopUpdate();
	}
}

void World::Pause()
{
	paused = !paused;
}

void World::Exit()
{
	shouldClose = true;
}

double World::GetLastFrameTime()
{
	return deltaTime;
}

void World::ComputeFrameDeltaTime()
{
	elapsedTime = Engine::GetElapsedTime();
	deltaTime = elapsedTime - previousTime;
	previousTime = elapsedTime;
}

void World::LoopUpdate()
{
	/* Poll and process events */
	window->PollEvents();

	ComputeFrameDeltaTime();
	if (paused) return;

	window->UpdateObservers();

	/* Frame */
	FrameStart();
	Update(static_cast<float>(deltaTime));
	FrameEnd();

	/* Swap front and back buffers */
	if (!paused)
		window->SwapBuffers();
}