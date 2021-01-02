#pragma once

#include "core/gpu/vertex_format.h"

#include "utils/gl_utils.h"
#include "utils/glm_utils.h"

#include <vector>


class GPUBuffers
{
	public:
		GPUBuffers();

		void CreateBuffers(unsigned int size);
		void ReleaseMemory();

	public:
		GLuint m_VAO;
		GLuint m_VBO[6];

	private:
		unsigned short m_size;
};

namespace gpu_utils
{

	GPUBuffers UploadData(const std::vector<glm::vec3> &positions,
							const std::vector<glm::vec3> &normals,
							const std::vector<unsigned short>& indices);

	GPUBuffers UploadData(const std::vector<glm::vec3> &positions,
							const std::vector<glm::vec3> &normals,
							const std::vector<glm::vec2> &text_coords,
							const std::vector<unsigned short> &indices);

	GPUBuffers UploadData(const std::vector<VertexFormat> &vertices,
							const std::vector<unsigned short>& indices);
}
