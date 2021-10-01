#pragma once

#include <string>
#include <vector>

#include "core/gpu/vertex_format.h"
#include "core/gpu/texture2D.h"
#include "core/gpu/gpu_buffers.h"

#include "assimp/scene.h"   // Output data structure


class Material
{
 public:
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

static const unsigned int INVALID_MATERIAL = std::numeric_limits<unsigned int>::max();

class MeshEntry
{
 public:
    MeshEntry()
    {
        nrIndices = 0;
        baseVertex = 0;
        baseIndex = 0;
        materialIndex = INVALID_MATERIAL;
    }
    unsigned int nrIndices;
    unsigned int baseVertex;
    unsigned int baseIndex;
    unsigned int materialIndex;
};

class Mesh
{
    typedef unsigned int GLenum;

 public:
    explicit Mesh(std::string meshID);
    virtual ~Mesh();

    void ClearData();

    // Initializes the mesh object using a VAO GPU buffer that contains the specified number of indices
    bool InitFromBuffer(unsigned int VAO,
                        unsigned int nrIndices);

    // Initializes the mesh object and upload data to GPU using the provided data buffers
    bool InitFromData(const std::vector<VertexFormat> &vertices,
                      const std::vector<unsigned int>& indices);

    // Initializes the mesh object and upload data to GPU using the provided data buffers
    bool InitFromData(const std::vector<glm::vec3>& positions,
                      const std::vector<glm::vec3>& normals,
                      const std::vector<unsigned int>& indices);

    // Initializes the mesh object and upload data to GPU using the provided data buffers
    bool InitFromData(const std::vector<glm::vec3>& positions,
                      const std::vector<glm::vec3>& normals,
                      const std::vector<glm::vec2>& texCoords,
                      const std::vector<unsigned int>& indices);

    bool LoadMesh(const std::string& fileLocation,
                  const std::string& fileName);

    void UseMaterials(bool value);

    // GL_POINTS, GL_TRIANGLES, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY,
    // GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY
    void SetDrawMode(GLenum primitive);
    GLenum GetDrawMode() const;

    void Render() const;

    const GPUBuffers* GetBuffers() const;
    const char* GetMeshID() const;

 protected:
    void InitFromData();

    void InitMesh(const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene);
    bool InitFromScene(const aiScene* pScene);

 private:
    std::string meshID;

 public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

 protected:
    std::string fileLocation;

    bool useMaterial;
    GLenum glDrawMode;
    GPUBuffers *buffers;

    std::vector<MeshEntry> meshEntries;
    std::vector<Material*> materials;
};
