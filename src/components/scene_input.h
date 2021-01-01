#pragma once

#include "components/simple_scene.h"

#include "core/window/input_controller.h"


class SceneInput : public InputController
{
	public:
		SceneInput(SimpleScene *camera);
		void OnKeyPress(int key, int mods) override;

	public:
		SimpleScene *scene;
};
