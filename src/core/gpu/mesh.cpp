#include "core/gpu/mesh.h"

#include <utility>

#include "assimp/Importer.hpp"          // C++ importer interface
#include "assimp/postprocess.h"         // Post processing flags

#include "core/gpu/gpu_buffers.h"
#include "core/gpu/texture2D.h"
#include "core/managers/texture_manager.h"

#include "utils/memory_utils.h"


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

    ClearAnimations(anim, numAnim);
    ClearRootNode(rootNode);
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
    bones.clear();
    m_BoneInfo.clear();
    m_BoneMapping.clear();
    m_BoneInfo.clear();
}

void Mesh::DeleteAnimationKeys(aiNodeAnim* nodeAnim) {
    delete[] nodeAnim->mPositionKeys;
    delete[] nodeAnim->mRotationKeys;
    delete[] nodeAnim->mScalingKeys;
}

void Mesh::ClearAnimations(aiAnimation** animations, unsigned int numAnimations)
{
    for (unsigned int animIndex = 0; animIndex < numAnimations; ++animIndex) {
        aiAnimation* animation = animations[animIndex];

        for (unsigned int channelIndex = 0; channelIndex < animation->mNumChannels; ++channelIndex) {
            DeleteAnimationKeys(animation->mChannels[channelIndex]);
            delete animation->mChannels[channelIndex];
        }

        delete[] animation->mChannels;
        delete animation;
    }

    delete[] animations;
}   

void Mesh::ClearRootNode(aiNode* node)
{
    for (unsigned int childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
        ClearRootNode(node->mChildren[childIndex]);
    }

    // Release memory for mMeshes array
    delete[] node->mMeshes;

    // Release memory for mChildren array and the node itself
    for (unsigned int childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
        delete node->mChildren[childIndex];
    }

    delete[] node->mChildren;
    delete node;
}

bool Mesh::LoadMesh(const std::string& fileLocation,
    const std::string& fileName)
{
    ClearData();
    this->fileLocation = fileLocation;
    std::string file = (fileLocation + '/' + fileName).c_str();

    Assimp::Importer Importer;

    unsigned int flags = aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
    if (glDrawMode == GL_TRIANGLES) flags |= aiProcess_Triangulate;

    const aiScene* pScene = Importer.ReadFile(file, flags);

    if (pScene) {
        m_GlobalInverseTransform = glm::inverse(ConvertMatrix(pScene->mRootNode->mTransformation));
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

    M.nrIndices = (unsigned int)indices.size();
    meshEntries.push_back(M);

    buffers->ReleaseMemory();
}


bool Mesh::InitFromBuffer(unsigned int VAO,
                          unsigned int nrIndices)
{
    if (VAO == 0 || nrIndices == 0)
        return false;

    meshEntries.clear();

    MeshEntry M;
    M.nrIndices = nrIndices;
    meshEntries.push_back(M);

    buffers->ReleaseMemory();
    buffers->m_VAO = VAO;

    return true;
}


bool Mesh::InitFromData(const std::vector<VertexFormat> &vertices,
                        const std::vector<unsigned int>& indices)
{
    this->vertices = vertices;
    this->indices = indices;

    InitFromData();
    *buffers = gpu_utils::UploadData(vertices, indices);
    return buffers->m_VAO != 0;
}


bool Mesh::InitFromData(const std::vector<glm::vec3>& positions,
                        const std::vector<glm::vec3>& normals,
                        const std::vector<unsigned int>& indices)
{
    this->positions = positions;
    this->normals = normals;
    this->indices = indices;

    InitFromData();
    *buffers = gpu_utils::UploadData(positions, normals, indices);
    return buffers->m_VAO != 0;
}


bool Mesh::InitFromData(const std::vector<glm::vec3>& positions,
                        const std::vector<glm::vec3>& normals,
                        const std::vector<glm::vec2>& texCoords,
                        const std::vector<unsigned int>& indices)
{
    this->positions = positions;
    this->normals = normals;
    this->texCoords = texCoords;
    this->indices = indices;

    InitFromData();
    *buffers = gpu_utils::UploadData(positions, normals, texCoords, indices);
    return buffers->m_VAO != 0;
}

bool Mesh::InitFromScene(const aiScene* pScene)
{
    
    CopyAnimations(pScene);
    rootNode = CopyRoot(pScene->mRootNode);

    meshEntries.resize(pScene->mNumMeshes);
    materials.resize(pScene->mNumMaterials);

    unsigned int nrVertices = 0;
    unsigned int nrIndices = 0;

    // Count the number of vertices and indices
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
    {
        meshEntries[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
        meshEntries[i].nrIndices = (pScene->mMeshes[i]->mNumFaces * (glDrawMode == GL_TRIANGLES ? 3 : 4));
        meshEntries[i].baseVertex = nrVertices;
        meshEntries[i].baseIndex = nrIndices;

        nrVertices += pScene->mMeshes[i]->mNumVertices;
        nrIndices += meshEntries[i].nrIndices;
    }

    // Reserve space in the vectors for the vertex attributes and indices
    positions.reserve(nrVertices);
    normals.reserve(nrVertices);
    texCoords.reserve(nrVertices);
    bones.resize(nrVertices);
    indices.reserve(nrIndices);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < meshEntries.size(); i++)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    if (useMaterial && !InitMaterials(pScene))
        return false;

    buffers->ReleaseMemory();
    *buffers = gpu_utils::UploadData(positions, normals, texCoords, bones, indices);
    return buffers->m_VAO != 0;
}

void Mesh::CopyAnimations(const aiScene* pScene)
{
    // Create a new aiAnimation instance for the destination animation
    numAnim = pScene->mNumAnimations;
    anim = new aiAnimation*[pScene->mNumAnimations];

    for (unsigned int i = 0; i < pScene->mNumAnimations; i++)
    {
        // Copy animation properties
        anim[i] = new aiAnimation();
        anim[i]->mName = pScene->mAnimations[i]->mName;
        anim[i]->mDuration = pScene->mAnimations[i]->mDuration;
        anim[i]->mTicksPerSecond = pScene->mAnimations[i]->mTicksPerSecond;

        anim[i]->mNumChannels = pScene->mAnimations[i]->mNumChannels;
        anim[i]->mChannels = new aiNodeAnim * [anim[i]->mNumChannels];

        // Copy position, rotation, and scaling keyframes
        for (int j = 0; j < anim[i]->mNumChannels; j++)
        {
            anim[i]->mChannels[j] = new aiNodeAnim();
            anim[i]->mChannels[j]->mNodeName = pScene->mAnimations[i]->mChannels[j]->mNodeName;
            anim[i]->mChannels[j]->mPostState = pScene->mAnimations[i]->mChannels[j]->mPostState;
            anim[i]->mChannels[j]->mPreState = pScene->mAnimations[i]->mChannels[j]->mPreState;

            anim[i]->mChannels[j]->mNumPositionKeys = pScene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
            anim[i]->mChannels[j]->mPositionKeys = new aiVectorKey[anim[i]->mChannels[j]->mNumPositionKeys];
            memcpy(anim[i]->mChannels[j]->mPositionKeys, pScene->mAnimations[i]->mChannels[j]->mPositionKeys,
                anim[i]->mChannels[j]->mNumPositionKeys * sizeof(aiVectorKey));

            anim[i]->mChannels[j]->mNumRotationKeys = pScene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
            anim[i]->mChannels[j]->mRotationKeys = new aiQuatKey[anim[i]->mChannels[j]->mNumRotationKeys];
            memcpy(anim[i]->mChannels[j]->mRotationKeys, pScene->mAnimations[i]->mChannels[j]->mRotationKeys,
                anim[i]->mChannels[j]->mNumRotationKeys * sizeof(aiQuatKey));

            anim[i]->mChannels[j]->mNumScalingKeys = pScene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
            anim[i]->mChannels[j]->mScalingKeys = new aiVectorKey[anim[i]->mChannels[j]->mNumScalingKeys];
            memcpy(anim[i]->mChannels[j]->mScalingKeys, pScene->mAnimations[i]->mChannels[j]->mScalingKeys,
                anim[i]->mChannels[j]->mNumScalingKeys * sizeof(aiVectorKey));
        }

        anim[i]->mNumMeshChannels = pScene->mAnimations[i]->mNumMeshChannels;
        anim[i]->mMeshChannels = new aiMeshAnim * [anim[i]->mNumMeshChannels];

        for (int j = 0; j < anim[i]->mNumMeshChannels; j++)
        {
            anim[i]->mMeshChannels[j] = new aiMeshAnim();
            anim[i]->mMeshChannels[j]->mName = pScene->mAnimations[i]->mMeshChannels[j]->mName;
            anim[i]->mMeshChannels[j]->mNumKeys = pScene->mAnimations[i]->mMeshChannels[j]->mNumKeys;
            anim[i]->mMeshChannels[j]->mKeys = new aiMeshKey(pScene->mAnimations[i]->mMeshChannels[j]->mKeys->mTime,
                pScene->mAnimations[i]->mMeshChannels[j]->mKeys->mValue);
        }
    }
}

aiNode* Mesh::CopyRoot(const aiNode* sourceNode)
{
    // Create a new aiNode instance for the destination node
    aiNode* destNode = new aiNode();

    // Copy node properties
    destNode->mName = sourceNode->mName;
    destNode->mTransformation = sourceNode->mTransformation;
    destNode->mMeshes = new unsigned int[sourceNode->mNumMeshes];
    for (unsigned int i = 0; i < sourceNode->mNumMeshes; ++i) {
        destNode->mMeshes[i] = sourceNode->mMeshes[i];
    }
    destNode->mNumMeshes = sourceNode->mNumMeshes;
    destNode->mParent = nullptr; // Set parent to nullptr for now

    // Copy child nodes recursively
    destNode->mNumChildren = sourceNode->mNumChildren;
    destNode->mChildren = new aiNode * [sourceNode->mNumChildren];

    for (unsigned int childIndex = 0; childIndex < sourceNode->mNumChildren; ++childIndex) {
        destNode->mChildren[childIndex] = CopyRoot(sourceNode->mChildren[childIndex]);
        destNode->mChildren[childIndex]->mParent = destNode; // Set the parent for the child
    }

    return destNode;
}

void Mesh::InitMesh(int index, const aiMesh* paiMesh)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
        normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
        texCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
    }

    LoadBones(index, paiMesh);

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

void Mesh::LoadBones(int MeshIndex, const aiMesh* pMesh)
{
    for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
        unsigned int BoneIndex = 0;
        std::string BoneName(pMesh->mBones[i]->mName.data);

        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
            // Allocate an index for a new bone
            BoneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            m_BoneInfo.push_back(bi);
            m_BoneInfo[BoneIndex].boneOffset = ConvertMatrix(pMesh->mBones[i]->mOffsetMatrix);
            m_BoneMapping[BoneName] = BoneIndex;
        }
        else {
            BoneIndex = m_BoneMapping[BoneName];
        }

        for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            unsigned int VertexID = meshEntries[MeshIndex].baseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            bones[VertexID].AddBoneData(BoneIndex, Weight);
        }
    }
}

glm::mat4 Mesh::ConvertMatrix(const aiMatrix4x4& aiMat)
{
    return {
    aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
    aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
    aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
    aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
    };
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
            memcpy((void *)&materials[i]->ambient, &color, sizeof(color));

        if (aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
            memcpy((void *)&materials[i]->diffuse, &color, sizeof(color));

        if (aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS)
            memcpy((void *)&materials[i]->specular, &color, sizeof(color));

        if (aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_EMISSIVE, &color) == AI_SUCCESS)
            memcpy((void *)&materials[i]->emissive, &color, sizeof(color));
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
    glBindVertexArray(buffers->m_VAO);
    for (unsigned int i = 0; i < meshEntries.size(); i++)
    {
        if (useMaterial)
        {
            auto materialIndex = meshEntries[i].materialIndex;
            if (materialIndex != INVALID_MATERIAL && materials[materialIndex]->texture)
            {
                (materials[materialIndex]->texture)->BindToTextureUnit(GL_TEXTURE0);
            } else {
                TextureManager::GetTexture(static_cast<unsigned int>(0))->BindToTextureUnit(GL_TEXTURE0);
            }
        }

        glDrawElementsBaseVertex(glDrawMode, meshEntries[i].nrIndices,
            GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * meshEntries[i].baseIndex),
            meshEntries[i].baseVertex);
    }
    glBindVertexArray(0);
}
