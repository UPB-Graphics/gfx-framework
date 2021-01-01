#pragma once

#include "core/window/input_controller.h"
#include "components/camera.h"


class CameraInput : public InputController
{
	public:
		CameraInput(egx::comp::Camera *camera);
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

	public:
		egx::comp::Camera *camera;
};
