#pragma once

#include <Core/Window/InputController.h>

namespace EngineComponents
{
	class Camera;
}

class CameraInput : public InputController
{
	public:
		CameraInput(EngineComponents::Camera *camera);
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

	public:
		EngineComponents::Camera *camera;
};

