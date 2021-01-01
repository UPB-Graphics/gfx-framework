#include "InputController.h"

#include "../Engine.h"

InputController::InputController()
{
	window = Engine::GetWindow();
	window->SubscribeToEvents(this);
	isAttached = true;
}

InputController::~InputController()
{
}

bool InputController::IsActive() const
{
	return isAttached;
}

void InputController::SetActive(bool value)
{
	isAttached = value;
	value ? window->SubscribeToEvents(this) : window->UnsubscribeFromEvents(this);
}
