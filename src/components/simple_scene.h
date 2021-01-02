#pragma once

#include "components/camera.h"

#include "core/world.h"
#include "core/engine.h"
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "core/gpu/texture2D.h"
#include "core/managers/resource_path.h"
#include "core/managers/texture_manager.h"

#include "include/text_utils.h"
#include "include/window_utils.h"
#include "include/memory_utils.h"
#include "include/gl_utils.h"
#include "include/math_utils.h"

#include <unordered_map>


class SimpleScene : public World
{
	friend class SceneInput;

	public:
		SimpleScene();
		~SimpleScene();

		bool ToggleGroundPlane();

	protected:
		virtual void AddMeshToList(Mesh *mesh);
		virtual void DrawCoordinatSystem();
		virtual void DrawCoordinatSystem(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMaxtix);

		virtual void RenderMesh(Mesh * mesh, Shader * shader, glm::vec3 position, glm::vec3 scale = glm::vec3(1));
		virtual void RenderMesh(Mesh * mesh, glm::vec3 position, glm::vec3 scale = glm::vec3(1));

		virtual void RenderMesh2D(Mesh * mesh, Shader * shader, const glm::mat3 &modelMatrix);
		virtual void RenderMesh2D(Mesh * mesh, const glm::mat3 &modelMatrix, const glm::vec3 &color) const;

		virtual void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 &modelMatrix);

		virtual void ReloadShaders() const final;
		virtual egx::comp::Camera* GetSceneCamera() const final;
		virtual InputController* GetCameraInput() const final;

		void ClearScreen();

	private:
		void InitResources();

	protected:
		std::unordered_map<std::string, Mesh*> meshes;
		std::unordered_map<std::string, Shader*> shaders;

	private:
		egx::comp::Camera *camera;
		InputController *cameraInput;

		bool drawGroundPlane;
		Mesh *xozPlane;
		Mesh *simpleLine;
		egx::comp::Transform *objectModel;
};
