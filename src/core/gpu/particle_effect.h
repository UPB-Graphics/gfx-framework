#pragma once

#include <include/gl_utils.h>
#include <include/glm_utils.h>

#include "components/camera.h"
#include "components/transform.h"

#include "core/gpu/shader.h"
#include "core/gpu/texture2D.h"
#include "core/gpu/ssbo.h"

#include <vector>
#include <chrono>


template <class T>
class ParticleEffect
{
	public:
		ParticleEffect();
		virtual ~ParticleEffect();

		virtual void Generate(unsigned int particleCount, bool createLocalBuffer = false);
		virtual void FillRandomData(std::function<T(void)> generator);
		virtual void Render(egx::comp::Camera *camera, Shader *shader, unsigned int nrParticles = -1);

		virtual SSBO<T>* GetParticleBuffer() const
		{
			return particles;
		}

		virtual unsigned int GetSize() const
		{
			return particleCount;
		}

	public:
		egx::comp::Transform * source;

	protected:
		unsigned int particleCount;
		GLuint VAO;
		GLuint VBO;
		SSBO<T> *particles;
};


template <class T>
ParticleEffect<T>::ParticleEffect()
{
	source = new egx::comp::Transform();
	particles = nullptr;
}

template <class T>
ParticleEffect<T>::~ParticleEffect()
{
	SAFE_FREE(source);
	SAFE_FREE(particles);
}

template <class T>
void ParticleEffect<T>::Render(egx::comp::Camera *camera, Shader *shader, unsigned int nrParticles)
{
	// Bind MVP
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(source->GetModel()));
	glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->m_view));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(camera->m_projection));
	glUniform3fv(shader->loc_eye_pos, 1, glm::value_ptr(camera->m_transform->GetWorldPosition()));

	// Bind Particle Storage
	particles->BindBuffer(0);

	// Render Particles
	glBindVertexArray(VAO);
	glDrawElements(GL_POINTS, MIN(particleCount, nrParticles), GL_UNSIGNED_INT, 0);
}

template <class T>
void ParticleEffect<T>::Generate(unsigned int particleCount, bool createLocalBuffer)
{
	this->particleCount = particleCount;

	SAFE_FREE(particles);
	particles = new SSBO<T>(particleCount, createLocalBuffer);

	unsigned int *indices = new unsigned int[particleCount];
	unsigned int *p = indices;
	for (unsigned int i = 0; i < particleCount; i++)
	{
		*p = i;
		p++;
	}

	GLuint IBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, particleCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] indices;
}

template <class T>
void ParticleEffect<T>::FillRandomData(std::function<T(void)> generator)
{
	particles->ReadBuffer();
	auto data = const_cast<T*>(particles->GetBuffer());
	for (unsigned int i = 0; i < particleCount; i++) {
		data[i] = generator();
	}
	particles->SetBufferData(data);
}
