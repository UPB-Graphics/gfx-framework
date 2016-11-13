#include "Mesh.h"

#include <include/utils.h>

#include <Core/GPU/GPUBuffers.h>
#include <Core/GPU/Texture2D.h>
#include <Core/Managers/TextureManager.h>

using namespace std;

static_assert(sizeof(aiColor4D) == sizeof(glm::vec4), "WARNING! glm::vec4 and aiColor4D size differs!");

Mesh::Mesh(std::string meshID)
{
	this->meshID = std::move(meshID);

	useMaterial = true;
	glDrawMode = GL_TRIANGLES;
	buffers = new GPUBuffers();
}

Mesh::~Mesh()
{
	ClearData();
	meshEntries.clear();
	SAFE_FREE(buffers);
}

const GPUBuffers * Mesh::GetBuffers() const
{
	return buffers;
}

const char * Mesh::GetMeshID() const
{
	return meshID.c_str();
}

void Mesh::ClearData()
{
	for (unsigned int i = 0 ; i < materials.size() ; i++) {
		SAFE_FREE(materials[i]);
	}
	positions.clear();
	texCoords.clear();
	indices.clear();
	normals.clear();
}

bool Mesh::LoadMesh(const string& fileLocation, const string& fileName)
{
	ClearData();
	this->fileLocation = fileLocation;
	string file = (fileLocation + '/' + fileName).c_str();

	Assimp::Importer Importer;

	unsigned int flags = aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
	if (glDrawMode == GL_TRIANGLES) flags |= aiProcess_Triangulate;

	const aiScene* pScene = Importer.ReadFile(file, flags);

	if (pScene) {
		return InitFromScene(pScene);
	}

	// pScene is freed when returning because of Importer

	printf("Error parsing '%s': '%s'\n", file.c_str(), Importer.GetErrorString());
	return false;
}

void Mesh::InitFromData()
{
	meshEntries.clear();

	MeshEntry M;
	M.nrIndices = static_cast<unsigned short>(indices.size());
	meshEntries.push_back(M);

	buffers->ReleaseMemory();
}

bool Mesh::InitFromBuffer(unsigned int VAO, unsigned short nrIndices)
{
	if (VAO == 0 || nrIndices == 0)
		return false;

	meshEntries.clear();

	MeshEntry M;
	M.nrIndices = nrIndices;
	meshEntries.push_back(M);

	buffers->ReleaseMemory();
	buffers->VAO = VAO;

	return true;
}

bool Mesh::InitFromData(std::vector<VertexFormat> vertices, std::vector<unsigned short>& indices)
{
	this->vertices = vertices;
	this->indices = indices;

	InitFromData();
	*buffers = UtilsGPU::UploadData(vertices, indices);
	return buffers->VAO != 0;
}

bool Mesh::InitFromData(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<unsigned short>& indices)
{
	this->positions = positions;
	this->normals = normals;
	this->indices = indices;

	InitFromData();
	*buffers = UtilsGPU::UploadData(positions, normals, indices);
	return buffers->VAO != 0;
}

bool Mesh::InitFromData(vector<glm::vec3>& positions,
						vector<glm::vec3>& normals,
						vector<glm::vec2>& texCoords,
						vector<unsigned short>& indices)
{
	this->positions = positions;
	this->normals = normals;
	this->texCoords = texCoords;
	this->indices = indices;

	InitFromData();
	*buffers = UtilsGPU::UploadData(positions, normals, texCoords, indices);
	return buffers->VAO != 0;
}

bool Mesh::InitFromScene(const aiScene* pScene)
{

	meshEntries.resize(pScene->mNumMeshes);
	materials.resize(pScene->mNumMaterials);

	unsigned int nrVertices = 0;
	unsigned int nrIndices = 0;

	// Count the number of vertices and indices
	for (unsigned int i = 0 ; i < pScene->mNumMeshes ; i++)
	{
		meshEntries[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
		meshEntries[i].nrIndices = pScene->mMeshes[i]->mNumFaces * (glDrawMode == GL_TRIANGLES ? 3 : 4);
		meshEntries[i].baseVertex = nrVertices;
		meshEntries[i].baseIndex = nrIndices;

		nrVertices += pScene->mMeshes[i]->mNumVertices;
		nrIndices  += meshEntries[i].nrIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	positions.reserve(nrVertices);
	normals.reserve(nrVertices);
	texCoords.reserve(nrVertices);
	indices.reserve(nrIndices);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0 ; i < meshEntries.size() ; i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(paiMesh);
	}

	if (useMaterial && !InitMaterials(pScene))
		return false;

	buffers->ReleaseMemory();
	*buffers = UtilsGPU::UploadData(positions, normals, texCoords, indices);
	return buffers->VAO != 0;
}

void Mesh::InitMesh(const aiMesh* paiMesh)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		texCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	// Init the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		indices.push_back(Face.mIndices[0]);
		indices.push_back(Face.mIndices[1]);
		indices.push_back(Face.mIndices[2]);
		if (Face.mNumIndices == 4)
			indices.push_back(Face.mIndices[3]);
	}
}

bool Mesh::InitMaterials(const aiScene* pScene)
{
	bool ret = true;
	aiColor4D color;

	for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];
		materials[i] = new Material();

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				materials[i]->texture = TextureManager::LoadTexture(fileLocation, Path.data);
			}
		}

		if (aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS)
			memcpy(&materials[i]->ambient, &color, sizeof(color));

		if (aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
			memcpy(&materials[i]->diffuse, &color, sizeof(color));

		if (aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS)
			memcpy(&materials[i]->specular, &color, sizeof(color));

		if (aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_EMISSIVE, &color) == AI_SUCCESS)
			memcpy(&materials[i]->emissive, &color, sizeof(color));
	}

	CheckOpenGLError();
	return ret;
}

GLenum Mesh::GetDrawMode() const
{
	return glDrawMode;
}

void Mesh::SetDrawMode(GLenum primitive)
{
	glDrawMode = primitive;
}

void Mesh::UseMaterials(bool value)
{
	useMaterial = value;
}

void Mesh::Render() const
{
	glBindVertexArray(buffers->VAO);
	for (unsigned int i = 0; i < meshEntries.size(); i++)
	{
		if (useMaterial)
		{
			auto materialIndex = meshEntries[i].materialIndex;
			if (materialIndex != INVALID_MATERIAL && materials[materialIndex]->texture)
			{
				(materials[materialIndex]->texture)->BindToTextureUnit(GL_TEXTURE0);
			}
			else {
				TextureManager::GetTexture(static_cast<unsigned int>(0))->BindToTextureUnit(GL_TEXTURE0);
			}
		}

		glDrawElementsBaseVertex(glDrawMode, meshEntries[i].nrIndices,
			GL_UNSIGNED_SHORT, (void*)(sizeof(unsigned short) * meshEntries[i].baseIndex),
			meshEntries[i].baseVertex);
	}
	glBindVertexArray(0);
}
