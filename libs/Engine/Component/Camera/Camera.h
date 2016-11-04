#pragma once
#include <vector>

#include <include/glm.h>
#include <DLL/dll_export.h>

namespace EngineComponents
{
	class Transform;

	enum class CameraType {
		FirstPerson,
		ThirdPerson
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

	class DLLExport Camera
	{
		public:
			Camera();
			~Camera();

			void Init();
			void Log() const;
			virtual void Update();

		public:
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
			void SetProjection(const ProjectionInfo &PI);
			ProjectionInfo GetProjectionInfo() const;
			float GetFieldOfViewY() const;
			float GetFieldOfViewX() const;

		protected:
			// FPS Rotation Mode
			void UpdatePitch(float deltaAngle);
			void SetYaw(float deltaAngle);
			void UpdateRoll(float deltaAngle);

		public:
			Transform *transform;

			// Camera Type
			CameraType type;

			glm::mat4 View;
			glm::mat4 Projection;

		protected:

			// Control settings
			float minSpeed;
			float maxSpeed;
			float sensitivityOX;
			float sensitivityOY;

			// Rotation OX constrains
			float limitUp;
			float limitDown;

			// Perspective properties
			float zNear;
			float zFar;
			float FoVy;
			float aspectRatio;
			bool isPerspective;
			float ortographicWidth;
	};

}
