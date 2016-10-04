#pragma once

class Mesh;
class Shader;
class Camera;
class Transform;

#include <Core/World.h>

class SimpleScene : public World
{
	public:
		SimpleScene();
		~SimpleScene();

	protected:
		void DrawCoordinatSystem();
		void RenderMesh(Mesh * mesh, Shader * shader, glm::vec3 position, glm::vec3 scale = glm::vec3(1));
		void RenderMesh(Mesh * mesh, glm::vec3 position, glm::vec3 scale = glm::vec3(1));

	private:
		void InitResources();

	protected:
		Camera *camera;
		std::unordered_map<std::string, Mesh*> meshes;
		std::unordered_map<std::string, Shader*> shaders;

	private:
		Mesh *xozPlane;
		Mesh *simpleLine;
		Transform *objectModel;
};