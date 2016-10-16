#pragma once

#include <Core/Window/InputController.h>

class SimpleScene;

class SceneInput : public InputController
{
	public:
		SceneInput(SimpleScene *camera);
		void OnKeyPress(int key, int mods) override;

	public:
		SimpleScene *scene;
};

