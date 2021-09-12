#ifndef GFXC_TRANSFORM_H
#define GFXC_TRANSFORM_H

#include "components/exports.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include <list>


namespace gfxc
{
    class GFXC_API Transform
    {
     public:

        Transform();
        Transform(const Transform &transform);
        virtual ~Transform();

        // ****************************
        // Transform motion state

        void SetHierarchyUpdate(bool value);
        void ClearMotionState();
        bool GetMotionState() const;

        // ****************************
        // Get transform properties

        virtual glm::vec3 GetLocalPosition() const;
        virtual glm::vec3 GetWorldPosition() const;
        virtual glm::quat GetWorldRotation() const;
        virtual glm::quat GetRelativeRotation() const;
        virtual glm::vec3 GetRotationEulerRad() const;
        virtual glm::vec3 GetRotationEuler360() const;

        virtual glm::vec3 GetLocalOYVector() const;
        virtual glm::vec3 GetLocalOXVector() const;
        virtual glm::vec3 GetLocalOZVector() const;

        virtual glm::vec3 GetScale() const;
        virtual const glm::mat4& GetModel();

        virtual float GetMoveSpeed() const;
        virtual float GetScaleSpeed() const;
        virtual float GetRotationSpeed() const;

        // ****************************
        // Continuous transform methods

        virtual void Move(const glm::vec3 &offset);
        virtual void Move(const glm::vec3 &dir, float deltaTime);
        virtual void Scale(float deltaTime);

        // Rotations
        virtual void RotateWorldOX(float deltaTime);
        virtual void RotateWorldOY(float deltaTime);
        virtual void RotateWorldOZ(float deltaTime);
        virtual void RotateLocalOX(float deltaTime);
        virtual void RotateLocalOY(float deltaTime);
        virtual void RotateLocalOZ(float deltaTime);

        // Positions
        virtual void SetLocalPosition(glm::vec3 position);
        virtual void SetWorldPosition(glm::vec3 position);

        // Rotations
        virtual void SetWorldRotation(glm::quat rotationQ);
        virtual void SetWorldRotation(const glm::vec3 &eulerAngles360);
        virtual void SetWorldRotationAndScale(const glm::quat &rotationQ, glm::vec3 scale);

        virtual void SetReleativeRotation(const glm::vec3 &eulerAngles360);
        virtual void SetReleativeRotation(const glm::quat &localRotationQ);

        // Scales
        virtual void SetScale(glm::vec3 scale);

        // ****************************
        // Transform properties

        virtual void SetMoveSpeed(float unitsPerSecond);
        virtual void SetScaleSpeed(float unitsPerSecond);
        virtual void SetRotationSpeed(float degreesPerSecond);

        // ****************************
        // Transform operations
        float DistanceTo(Transform *transform);
        float DistanceTo(const glm::vec3 &position);
        float Distance2To(Transform *transform);
        float Distance2To(const glm::vec3 &position);

        // ****************************
        // Hierarchy chain

        void AddChild(Transform *transform);
        void RemoveChild(Transform *transform);

     protected:
        virtual void Init();

        // ****************************
        // Should only be called by the owner

        virtual void ComputeWorldModel();
        virtual void UpdateWorldModel();
        virtual void UpdateWorldPosition();
        virtual void UpdateLocalPosition();
        virtual void UpdateRelativeRotation();

        virtual void UpdateWorldInfo();
        virtual void UpdateChildrenPosition();
        virtual void UpdateChildrenRotation();

     private:
        virtual void UpdateModelPosition();

     protected:
        glm::mat4               m_worldModel;

        // Rotations
        glm::quat               m_worldRotation;
        glm::quat               m_relativeRotation;
        glm::quat               m_invWorldRotation;

        // Positions
        glm::vec3               m_worldPosition;
        glm::vec3               m_localPosition;

        // Scale relative to the object, not influenced by rotations
        glm::vec3               m_localScalingFactor;

        // Speeds
        float                   m_rotationSpeed;
        float                   m_translationSpeed;
        float                   m_scalingSpeed;

        bool                    m_isInMotion;
        bool                    m_isModelOutdated;
        bool                    m_updateHierarchy;

        Transform *             m_parentNode;
        std::list<Transform *>  m_childNodes;
    };
}

#endif
