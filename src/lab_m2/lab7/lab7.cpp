#include "lab_m2/lab7/lab7.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab7::Lab7()
{
}


Lab7::~Lab7()
{
}


void Lab7::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 3.5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
    camera->Update();

    // Create a shader program for rendering to texture
    {
        Shader* shader = new Shader("Skinning");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Lab7", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Lab7", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("animation");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "skinning"), "boblampclean.md5mesh");
        meshes[mesh->GetMeshID()] = mesh;
    }
}


void Lab7::FrameStart()
{
}


void Lab7::Update(float deltaTimeSeconds)
{
    ClearScreen();
    auto shader = shaders["Skinning"];
    shader->Use();

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
    RenderSimpleMesh(meshes["animation"], shader, modelMatrix);

    float runningTime = (float)((double)Engine::GetElapsedTime());
    BoneTransform(meshes["animation"], runningTime);
}

void Lab7::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 bones[200];

    for (int i = 0; i < mesh->m_BoneInfo.size(); i++)
    {
        bones[i] = mesh->m_BoneInfo[i].finalTransformation;
    }

    // TODO (student): Send the bone final transformation to the shader

    mesh->Render();
}

void Lab7::BoneTransform(Mesh* mesh, float timeInSeconds)
{
    glm::mat4 Identity = glm::mat4(1.0f);

    // Compute the duration of the animation
    float ticksPerSecond = mesh->anim[0]->mTicksPerSecond != 0 ? mesh->anim[0]->mTicksPerSecond : 25.0f;
    float timeInTicks = timeInSeconds * ticksPerSecond;
    float animationTime = fmod(timeInTicks, mesh->anim[0]->mDuration);

    // Compute the final transformations for each bone at the current time stamp
    // starting from the root node
    // TODO (student): Uncomment this to test the animation
    //ReadNodeHierarchy(mesh, animationTime, mesh->rootNode, Identity, mesh->anim);
}

void Lab7::ReadNodeHierarchy(Mesh* mesh, float animationTime, const aiNode* pNode, const glm::mat4& parentTransform, aiAnimation** anim)
{
    std::string NodeName(pNode->mName.data);

    // Our model has only one animation, which is stored in anim[0].
    // If we had more animations, we would need to select what animation we want
    const aiAnimation* pAnimation = anim[0];

    glm::mat4 nodeTransformation(mesh->ConvertMatrix(pNode->mTransformation));

    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        // Interpolate the scaling and generate the scaling transformation matrix
        aiVector3D Scaling;
        ComputeInterpolatedScaling(Scaling, animationTime, pNodeAnim);
        glm::mat4 ScalingM = glm::transpose(glm::mat4(Scaling.x, 0, 0, 0,
            0, Scaling.y, 0, 0,
            0, 0, Scaling.z, 0,
            0, 0, 0, 1));

        // Interpolate the rotation and generate the rotation transformation matrix
        aiQuaternion RotationQ;
        ComputeInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
        glm::quat rotation(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
        glm::mat4 RotationM = glm::toMat4(rotation);

        // Interpolate the translation and generate the translation transformation matrix
        aiVector3D Translation;
        ComputeInterpolatedPosition(Translation, animationTime, pNodeAnim);
        glm::mat4 TranslationM = glm::transpose(glm::mat4(1, 0, 0, Translation.x,
            0, 1, 0, Translation.y,
            0, 0, 1, Translation.z,
            0, 0, 0, 1));

        // Combine the above transformations
        nodeTransformation = TranslationM * RotationM * ScalingM;
    }

    // TODO (student): Apply the parent transformation to the current transformation
    glm::mat4 GlobalTransformation = glm::mat4(1.0f);

    if (mesh->m_BoneMapping.find(NodeName) != mesh->m_BoneMapping.end()) {
        // Bring the vertices from their local space position into their node space.
        // Multiply the result with the combined transformations of all the node parents plus the current transformation.
        // Bring the result back into local space.

        unsigned int BoneIndex = mesh->m_BoneMapping[NodeName];
        mesh->m_BoneInfo[BoneIndex].finalTransformation = mesh->m_GlobalInverseTransform * GlobalTransformation *
            mesh->m_BoneInfo[BoneIndex].boneOffset;
    }

    // Compute the transformations of the children of the current node
    for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
        ReadNodeHierarchy(mesh, animationTime, pNode->mChildren[i], GlobalTransformation, anim);
    }
}

const aiNodeAnim* Lab7::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
    for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }

    return NULL;
}

unsigned int Lab7::FindRotation(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    // TODO (student): Inside the aiNodeAnim variable we have the mRotationKeys array which stores
    // the rotation transformation of the vertex at different time stamps. The data is sorted. Find
    // the last rotation transformation which is at a lower time stamp so that we can compute the
    // interpolation of the rotation transformation of the bone. Return the index.
    return 0;

    assert(0);
}

void Lab7::ComputeInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumRotationKeys == 1) {
        out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    // Find the time stamps at which we should interpolate
    unsigned int rotationIndex = FindRotation(animationTime, pNodeAnim);
    unsigned int nextRotationIndex = (rotationIndex + 1);
    assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);

    // TODO (student): Compute the duration between the two selected time stamps
    // HINT! An entry from the mRotationKeys array has an mTime field
    float deltaTime = 0.0f;

    // Compute the factor of interpolation for a frame
    float factor = (animationTime - (float)pNodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    // TODO (student): Compute the final rotation factor by interpolating the values from the two selected key frames
    // HINT! Use the Interpolate method from the aiQuaternion class! Don't forget to normalize the result!
    // Save the result in the out variable
}

unsigned int Lab7::FindScaling(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    // TODO (student): Inside the aiNodeAnim variable we have the mScalingKeys array which stores
    // the scaling transformation of the vertex at different time stamps. The data is sorted. Find
    // the last scale transformation which is at a lower time stamp so that we can compute the
    // interpolation of the scaling transformation of the bone. Return the index.
    return 0;

    assert(0);
}


void Lab7::ComputeInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    // Find the time stamps at which we should interpolate
    unsigned int scalingIndex = FindScaling(animationTime, pNodeAnim);
    unsigned int nextScalingIndex = (scalingIndex + 1);
    assert(nextScalingIndex < pNodeAnim->mNumScalingKeys);

    // TODO (student): Compute the duration between the two selected time stamps
    // HINT! An entry from the mScalingKeys array has an mTime field
    float deltaTime = 0.0f;

    // Compute the factor of interpolation for a frame
    float factor = (animationTime - (float)pNodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    // TODO (student): Compute the final scale factor by interpolating the values from the two selected key frames
    // Save the result in the out variable
}


unsigned int Lab7::FindPosition(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumPositionKeys > 0);

    // TODO (student): Inside the aiNodeAnim variable we have the mPositionKeys array which stores
    // the translation transformation of the vertex at different time stamps. The data is sorted. Find
    // the last translation transformation which is at a lower time stamp so that we can compute the
    // interpolation of the translation transformation of the bone. Return the index.
    return 0;

    assert(0);
}

void Lab7::ComputeInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    // Find the time stamps at which we should interpolate
    unsigned int positionIndex = FindPosition(animationTime, pNodeAnim);
    unsigned int nextPositionIndex = (positionIndex + 1);
    assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);

    // TODO (student): Compute the duration between the two selected time stamps
    // HINT! An entry from the mPositionKeys array has an mTime field
    float deltaTime = 0.0f;

    // Compute the factor of interpolation for a frame
    float factor = (animationTime - (float)pNodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    // TODO (student): Compute the final translation factor by interpolating the values from the two selected key frames
    // Save the result in the out variable
}

void Lab7::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab7::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input with window->KeyHold()

}


void Lab7::OnKeyPress(int key, int mods)
{
    // Add key press event

}


void Lab7::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab7::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab7::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab7::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab7::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab7::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
