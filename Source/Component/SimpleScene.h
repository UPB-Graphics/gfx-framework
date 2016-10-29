#pragma once

class Mesh;
class Shader;
class Camera;
class Transform;
class InputController;

#include <Core/World.h>

class SimpleScene : public World
{
	friend class SceneInput;

	public:
		SimpleScene();
		~SimpleScene();

	protected:
		void AddMeshToList(Mesh *mesh);
		void DrawCoordinatSystem();

		void RenderMesh(Mesh * mesh, Shader * shader, glm::vec3 position, glm::vec3 scale = glm::vec3(1));
		void RenderMesh(Mesh * mesh, glm::vec3 position, glm::vec3 scale = glm::vec3(1));

		void RenderMesh2D(Mesh * mesh, Shader * shader, const glm::mat3 &modelMatrix);
		void RenderMesh2D(Mesh * mesh, const glm::mat3 &modelMatrix, const glm::vec3 &color) const;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 &modelMatrix);

		void ReloadShaders() const;

	private:
		void InitResources();

	protected:
		Camera *camera;
		InputController *cameraInput;
		std::unordered_map<std::string, Mesh*> meshes;
		std::unordered_map<std::string, Shader*> shaders;

	private:
		bool drawGroundPlane;
		Mesh *xozPlane;
		Mesh *simpleLine;
		Transform *objectModel;
};