#pragma once

#include "window/input_controller.h"

#include <unordered_map>


class Mesh;
class Shader;

class World : public InputController
{
	public:
		World();
		virtual ~World() {};
		virtual void Init() {};
		virtual void FrameStart() {};
		virtual void Update(float deltaTimeSeconds) {};
		virtual void FrameEnd() {};

		virtual void Run() final;
		virtual void Pause() final;
		virtual void Exit() final;

		virtual double GetLastFrameTime() final;

	private:
		void ComputeFrameDeltaTime();
		void LoopUpdate();

	private:
		double previousTime;
		double elapsedTime;
		double deltaTime;
		bool paused;
		bool shouldClose;
};
