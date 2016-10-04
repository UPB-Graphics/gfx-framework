#include "Engine.h"

#include <iostream>

#include <include/gl.h>

using namespace std;

WindowObject* Engine::window = nullptr;

WindowObject* Engine::Init(WindowProperties & props)
{
	/* Initialize the library */
	if (!glfwInit())
		exit(0);

	window = new WindowObject(props);

	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Serious problem
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		#ifdef WIN32
		Sleep(2000);
		#endif
		exit(0);
	}

	TextureManager::Init();

	return window;
}

WindowObject* Engine::GetWindow()
{
	return window;
}

void Engine::Exit()
{
	cout << "=====================================================" << endl;
	cout << "Engine closed. Exit" << endl;
	glfwTerminate();
}

double Engine::GetElapsedTime()
{
	return glfwGetTime();
}