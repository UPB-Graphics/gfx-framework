#pragma once
#include <include/utils.h>
#include "WindowObject.h"

class InputController
{
	friend class WindowObject;

	public:
		InputController();
		virtual ~InputController();

		bool IsActive() const;
		void SetActive(bool value);

	protected:
		// Method will be called each frame before the Update() method
		// Use for real-time frame-independent interaction such as performing continuous updates when pressing a key
		// @param deltaTime - frame time in seconds for the previous frame, may be used for frame time independent updates
		// @param mods - (modifiers) bit combination of GLFW_MOD_"KEY" states: CONTROL, ALT, SHIFT, SUPER
		//			   - the modifiers value holds information about the special keys pressed alongside the normal key event
		//			   - use for testing key combination such as: CONTROL + ALT + KEY, CONTROL + SHIFT + KEY, etc 
		// Use window->KeyHold(GLFW_KEY_"KEYCODE") for testing if a key is being pressed
		virtual void OnInputUpdate(float deltaTime, int mods) {};

		// If a KEY PRESS event is registered the method will be called before the Update() method
		// @param key - GLFW_KEY_"KEYCODE", where KEYCODE is a standard keyboard key
		// @param mods - bit combination of GLFW_MOD_* states: CONTROL, ALT, SHIFT, SUPER
		virtual void OnKeyPress(int key, int mods) {};

		// If a KEY RELEASE event is registered the method will be called before the Update() method
		// @param key - GLFW_KEY_"KEYCODE", where KEYCODE is a standard keyboard key
		// @param mods - bit combination of GLFW_MOD_* states: CONTROL, ALT, SHIFT, SUPER
		virtual void OnKeyRelease(int key, int mods) {};

		// If a MOUSE MOVE event is registered the method will be called before the Update() method
		// @param mouseX - X coordinate in pixels of the mouse position relative to the top-left corner ([0, 0])
		// @param mouseY - Y coordinate in pixels of the mouse position relative to the top-left corner
		// @param deltaX - offsetX in pixels from the last reported position
		// @param deltaY - offsetY in pixels from the last reported position
		virtual void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {};

		// If a MOUSE BUTTON PRESS event is registered the method will be called before the Update() method
		// @param mouseX - X coordinate in pixels of the mouse position relative to the top-left corner ([0, 0])
		// @param mouseY - Y coordinate in pixels of the mouse position relative to the top-left corner
		// @param button - bit-mask with the buttons currently pressed. Use the preprocessor helpers for testing:
		//				    
		//						IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)
		//						IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)
		//						IS_BIT_SET(button, GLFW_MOUSE_BUTTON_*) ... etc
		//
		// @param mods   - bit combination of GLFW_MOD_* states: CONTROL, ALT, SHIFT, SUPER
		virtual void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {};

		// Same as OnMouseBtnPress but triggered when a button is released
		virtual void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {};

		// When a MOUSE SCROLL event is registered the method will be called before the Update() method
		// @param mouseX - X coordinate in pixels of the mouse position relative to the top-left corner ([0, 0])
		// @param mouseY - Y coordinate in pixels of the mouse position relative to the top-left corner
		// @param offsetX - scroll steps on X axis
		// @param offsetY - scroll steps on Y axis 
		virtual void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {};

		// If the window was resized during the last frame this method will be before the Update() method
		// @param width - the new width of the window
		// @param height - the new height of the window
		virtual void OnWindowResize(int width, int height) {};

	protected:
		WindowObject *window;

	private:
		bool isAttached;
};