#include "core/window/input_controller.h"

#include "core/engine.h"


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
