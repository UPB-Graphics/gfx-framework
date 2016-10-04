#include "InputController.h"

#include "../Engine.h"

InputController::InputController()
{
	window = Engine::GetWindow();
	window->Subscribe(this);
}

InputController::~InputController()
{
}