#pragma once
#include <string>
#include <vector>

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags

#include <include/glm.h>

class GPUBuffers;
class Texture2D;

struct VertexFormat
{
	VertexFormat(glm::vec3 position, glm::vec3 normal,
				glm::uvec3 color = glm::uvec3(0),
				glm::uvec2 text_coord = glm::uvec2(0))
		: position(position), normal(normal), color(color), text_coord(text_coord)	{ }

	// position of the vertex
	glm::vec3 position;		

	// vertex normal
	glm::vec3 normal;

	// vertex color
	glm::uvec3 color;

	// vertex texture coordinate
	glm::uvec2 text_coord;
};

struct Material
{
	Material()
	{
		texture = nullptr;
	}

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emissive;
	float shininess;

	Texture2D* texture;
};

static const unsigned int INVALID_MATERIAL = 0xFFFFFFFF;

struct MeshEntry
{
	MeshEntry()
	{
		nrIndices = 0;
		baseVertex = 0;
		baseIndex = 0;
		materialIndex = INVALID_MATERIAL;
	}
	unsigned short nrIndices;
	unsigned short baseVertex;
	unsigned short baseIndex;
	unsigned int materialIndex;
};

class Mesh
{
	public:
		Mesh(std::string meshID);
		virtual ~Mesh();

		void ClearData();

		bool InitFromData(std::vector<VertexFormat> vertices,
						std::vector<unsigned short>& indices);
		bool InitFromData(std::vector<glm::vec3>& positions,
						std::vector<glm::vec3>& normals,
						std::vector<unsigned short>& indices);
		bool InitFromData(std::vector<glm::vec3>& positions,
						std::vector<glm::vec3>& normals,
						std::vector<glm::vec2>& texCoords,
						std::vector<unsigned short>& indices);

		bool LoadMesh(const std::string& fileLocation, const std::string& fileName);

		void UseMaterials(bool value);

		// GL_POINTS, GL_LINES, GL_TRIANGLES
		void SetDrawMode(unsigned int drawMode);
		void Render() const;

		const char* GetMeshID() const;

	protected:
		void InitFromData();
		void UploadVertexData();

		void InitMesh(const aiMesh* paiMesh);
		bool InitMaterials(const aiScene* pScene);
		bool InitFromScene(const aiScene* pScene);

	private:
		std::string meshID;
		glm::vec3 halfSize;
		glm::vec3 meshCenter;

	public:
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> colors;
		std::vector<glm::vec2> texCoords;
		std::vector<VertexFormat> vertices;
		std::vector<unsigned short> indices;

	protected:
		std::string fileLocation;

		bool useMaterial;
		unsigned int glDrawMode;
		GPUBuffers *buffers;

		std::vector<MeshEntry> meshEntries;
		std::vector<Material*> materials;
};