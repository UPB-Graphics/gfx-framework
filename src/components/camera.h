#ifndef GFXC_CAMERA_H
#define GFXC_CAMERA_H

#include "components/exports.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include <vector>


namespace gfxc
{
    class Transform;

    enum class CameraType {
        FIRST_PERSON,
        THIRD_PERSON
    };

    struct ProjectionInfo
    {
        float FoVy;
        float zNear;
        float zFar;
        float aspectRatio;
        float width;
        float height;
        bool isPerspective;
    };

    class GFXC_API Camera
    {
     public:
        Camera();
        ~Camera();

        void Init();
        void Log() const;
        virtual void Update();

        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetProjectionMatrix() const;

        // Rotation
        void RotateOX(float deltaTime);
        void RotateOY(float deltaTime);
        void RotateOZ(float deltaTime);

        // Speed
        void UpdateSpeed(float offset = 0.2f);

        // Translate Camera - local axis
        void SetPosition(const glm::vec3 &position);
        void SetRotation(const glm::quat &worldRotation);
        void SetPositionAndRotation(const glm::vec3 &position, const glm::quat &worldRotation);

        // Translate Camera - world axis
        void MoveForward(float deltaTime);
        void MoveBackward(float deltaTime);
        void MoveRight(float deltaTime);
        void MoveLeft(float deltaTime);
        void MoveUp(float deltaTime);
        void MoveDown(float deltaTime);
        void MoveInDirection(glm::vec3 direction, float deltaTime);

        void SetPerspective(float FoVy, float aspectRatio, float zNear, float zFar);
        void SetOrthographic(float width, float height, float zNear, float zFar);
        void SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);

        void SetProjection(const ProjectionInfo& projInfo);
        ProjectionInfo GetProjectionInfo() const;

        float GetFieldOfViewY() const;
        float GetFieldOfViewX() const;

     protected:
        // FPS Rotation Mode
        void UpdateRoll(float deltaAngle);
        void UpdateYaw(float deltaAngle);
        void UpdatePitch(float deltaAngle);

     public:
        // Other components can be public, as we're not using inheritance
        Transform *     m_transform;

     protected:
        // Camera Type
        CameraType      m_type;

        glm::mat4       m_view;
        glm::mat4       m_projection;

        // Control settings
        float           m_minSpeed;
        float           m_maxSpeed;
        float           m_sensitivityOX;
        float           m_sensitivityOY;

        // Rotation OX constrains
        float           m_limitUp;
        float           m_limitDown;

        // Perspective properties
        float           m_zNear;
        float           m_zFar;
        float           m_FoVy;
        float           m_aspectRatio;
        bool            m_isPerspective;
        float           m_ortographicWidth;
    };
}

#endif
