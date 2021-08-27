#pragma once

#include "components/simple_scene.h"
#include "core/window/input_controller.h"


namespace gfxc
{
    class SceneInput : public InputController
    {
     public:
        explicit SceneInput(SimpleScene *camera);
        void OnKeyPress(int key, int mods) override;

     public:
        SimpleScene *scene;
    };
}
