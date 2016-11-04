#pragma once
#include <list>

#include <include/glm.h>
#include <DLL/dll_export.h>

namespace EngineComponents
{
	class DLLExport Transform
	{
		public:
			Transform();
			Transform(const Transform &trasform);
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

			virtual void SetScale(glm::vec3 scale);
			virtual void ForceUpdate();

			// ****************************
			// Transform properties

			virtual void Copy(const Transform &source);
			virtual void SetMoveSpeed(float unitsPerSecond);
			virtual void SetScaleSpeed(float unitsPerSecond);
			virtual void SetRotationSpeed(float degreesPerSecond);

			// ****************************
			// Hierarchy chain

			void AddChild(Transform *transform);
			void RemoveChild(Transform *transform);

			// ****************************
			// Transform operations
			float DistanceTo(Transform *transform);
			float DistanceTo(const glm::vec3 &position);
			float Distance2To(Transform *transform);
			float Distance2To(const glm::vec3 &position);
			glm::vec3 GetRelativePositionOf(const Transform &transform);
			//glm::vec3 GetDirectionTowards(Transform *transform);

		protected:
			virtual void Init();

			// ****************************
			// Should be called only by the owner

			virtual void ComputeWorldModel();
			virtual void UpdateWorldModel();
			virtual void UpdateWorldPosition();
			virtual void UpdateLocalPosition();
			virtual void UpdateRelativeRotation();

			virtual void UpdateWorldInfo();
			virtual void UpdateChildsPosition();
			virtual void UpdateChildrenRotation();

		private:
			virtual void UpdateModelPosition();

		protected:
			glm::mat4 _worldModel;

			// Rotations
			glm::quat _worldRotation;
			glm::quat _relativeRotation;
			glm::quat _invWorldRotation;

			// Positions
			glm::vec3 _worldPosition;
			glm::vec3 _localPosition;

			// Scale relative to the object, not influenced by rotations
			glm::vec3 _localScale;

			float _rotateSpeed;
			float _moveSpeed;
			float _scaleSpeed;

			bool _motionState;
			bool _modelIsOutdated;
			bool _updateHierarchy;

			Transform* _parentNode;
			std::list<Transform*> _childNodes;
	};
}