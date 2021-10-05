#pragma once

#include <string>
#include <unordered_map>

#include "components/camera.h"

#include "core/world.h"
#include "core/engine.h"
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "core/gpu/texture2D.h"
#include "core/managers/resource_path.h"
#include "core/managers/texture_manager.h"

#include "utils/text_utils.h"
#include "utils/memory_utils.h"
#include "utils/gl_utils.h"
#include "utils/math_utils.h"


namespace gfxc
{
    class SimpleScene : public World
    {
        friend class SceneInput;

     public:
        SimpleScene();
        ~SimpleScene();

        bool ToggleGroundPlane();
        void ReloadShaders() const;

        protected:
        virtual void AddMeshToList(Mesh *mesh);
        virtual void DrawCoordinateSystem();
        virtual void DrawCoordinateSystem(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMaxtix);

        virtual void RenderMesh(Mesh *mesh, Shader *shader, glm::vec3 position, glm::vec3 scale = glm::vec3(1));
        virtual void RenderMesh(Mesh *mesh, glm::vec3 position, glm::vec3 scale = glm::vec3(1));

        virtual void RenderMesh2D(Mesh *mesh, Shader *shader, const glm::mat3 &modelMatrix);
        virtual void RenderMesh2D(Mesh *mesh, const glm::mat3 &modelMatrix, const glm::vec3 &color) const;

        virtual void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);

        Camera *GetSceneCamera() const;
        InputController *GetCameraInput() const;

        void ClearScreen(const glm::vec3 &color = glm::vec3(0, 0, 0));

     private:
        void InitResources();
        void Update(float deltaTimeSeconds) override;

        protected:
        std::unordered_map<std::string, Mesh *> meshes;
        std::unordered_map<std::string, Shader *> shaders;

        /*
         * The OpenGL implementation of `glLineWidth` on Apple devices
         * does not allow line widths greater than 1. As a workaround,
         * we shadow the native `glLineWidth` API call with our own.
         * See https://stackoverflow.com/a/8794400 for more details.
         */
        void glLineWidth(GLfloat width);

     private:
        Camera *camera;
        InputController *cameraInput;

        bool drawGroundPlane;
        Mesh *xozPlane;
        Mesh *simpleLine;
        Transform *objectModel;
    };
}
