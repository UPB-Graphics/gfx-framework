#pragma once

class Mesh;
class Shader;
class InputController;

namespace EngineComponents
{
	class Camera;
	class Transform;
}

#include <Core/World.h>

class SimpleScene : public World
{
	friend class SceneInput;

	public:
		SimpleScene();
		~SimpleScene();

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
		virtual EngineComponents::Camera* GetSceneCamera() const final;
		virtual InputController* GetCameraInput() const final;

	private:
		void InitResources();

	protected:
		std::unordered_map<std::string, Mesh*> meshes;
		std::unordered_map<std::string, Shader*> shaders;

	private:
		EngineComponents::Camera *camera;
		InputController *cameraInput;

		bool drawGroundPlane;
		Mesh *xozPlane;
		Mesh *simpleLine;
		EngineComponents::Transform *objectModel;
};