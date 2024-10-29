#pragma once

#include <string>
#include <list>

#include "core/window/input_controller.h"
#include "core/window/window_callbacks.h"

#include "utils/glm_utils.h"


class WindowProperties
{
 public:
    WindowProperties();

 public:
    std::string selfDir;
    std::string name;
    glm::ivec2 resolution;
    float scaleFactor;
    glm::ivec2 position;
    glm::ivec2 cursorPos;
    float aspectRatio;
    bool resizable;
    bool visible;
    bool fullScreen;
    bool centered;
    bool hideOnClose;
    bool vSync;
};


/*
 *  Opaque window handle
 */
struct WindowDataImpl;
typedef WindowDataImpl *WindowData;

/*
 * Class WindowObject
 */

class WindowObject {
   friend class InputController;
   friend class WindowCallbacks;

public:
   explicit WindowObject(WindowProperties properties);
   ~WindowObject();

   void Show();
   void Hide();
   void Close();
   int ShouldClose() const;

   void ShowPointer();
   void CenterPointer();
   void SetPointerPosition(int mousePosX, int mousePosY);
   void HidePointer();
   void DisablePointer();

   void SetWindowPosition(glm::ivec2 position);
   void CenterWindow();

   void SwapBuffers() const;
   void SetVSync(bool state);
   bool ToggleVSync();

   void MakeCurrentContext() const;

   // Window Information
   void SetSize(int width, int height);

   // Use scaled resolution for setting the viewport.
   // Use unscaled resolution when working with mouse coordinates.
   glm::ivec2 GetResolution(bool unscaled = false) const;

   float GetAspectRatio() const;

   const std::string& GetSelfDir() const;

   // Window Event
   void PollEvents() const;

   // Get Input State
   bool KeyHold(int keyCode) const;
   bool MouseHold(int button) const;
   int GetSpecialKeyState() const;

   // Use unscaled resolution when working with mouse coordinates.
   glm::ivec2 GetCursorPosition() const;

   // Update event listeners (key press / mouse move / window events)
   void UpdateObservers();

protected:
   // Frame time
   void ComputeFrameTime();

   // Window Creation
   void FullScreen();
   void WindowMode();

   // Input Processing
   void KeyCallback(int key, int scanCode, int action, int mods);
   void MouseButtonCallback(int button, int action, int mods);
   void MouseMove(int posX, int posY);
   void MouseScroll(double offsetX, double offsetY);

   // Subscribe to receive input events
   void SubscribeToEvents(InputController * IC);
   void UnsubscribeFromEvents(InputController * IC);

private:
   void SetWindowCallbacks();

   WindowProperties props;
public:
   WindowData window;

private:
   // Frame Time
   unsigned int frameID;
   double elapsedTime;
   double deltaFrameTime;

   // Window state and events
   bool hiddenPointer;
   bool resizeEvent;

   // Mouse button callback
   int mouseButtonCallback;            // bit field for button callback
   int mouseButtonAction;              // bit field for button state
   int mouseButtonStates;              // bit field for mouse button state

   // Mouse move event
   bool mouseMoveEvent;
   int mouseDeltaX;
   int mouseDeltaY;

   // Mouse scroll event
   bool scrollEvent;
   int mouseScrollDeltaX;
   int mouseScrollDeltaY;

   // States for keyboard buttons - PRESSED(true) / RELEASED(false)
   int registeredKeyEvents;
   int keyEvents[128];
   bool keyStates[384];

   // Platform specific key codes - PRESSED(true) / RELEASED(false)
   bool keyScanCode[512];

   // Special keys (ALT, CTRL, SHIFT, CAPS LOOK, OS KEY) active alongside with normal key or mouse input
   int keyMods;

   // Input Observers
   std::list<InputController*> observers;
};
