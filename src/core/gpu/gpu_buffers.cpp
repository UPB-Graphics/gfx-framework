#include "core/gpu/gpu_buffers.h"
#include "core/gpu/vertex_format.h"


enum VERTEX_ATTRIBUTE_LOC
{
    POS,
    NORMAL,
    TEX_COORD,
};


GPUBuffers::GPUBuffers()
{
    m_size = 0;
    m_VAO = 0;
    memset(m_VBO, 0, 6 * sizeof(int));
}


void GPUBuffers::CreateBuffers(unsigned int size)
{
    this->m_size = size;
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(size, m_VBO);
}


void GPUBuffers::ReleaseMemory()
{
    if (m_size)
    {
        m_size = 0;
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(m_size, m_VBO);
    }
}


GPUBuffers gpu_utils::UploadData(const std::vector<glm::vec3> &positions,
                                 const std::vector<glm::vec3> &normals,
                                 const std::vector<unsigned int>& indices)
{
    GPUBuffers buffers;
    buffers.CreateBuffers(3);
    glBindVertexArray(buffers.m_VAO);

    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, buffers.m_VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOC::POS);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOC::POS, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.m_VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOC::NORMAL);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOC::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.m_VBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);

    CheckOpenGLError();

    return buffers;
}


GPUBuffers gpu_utils::UploadData(const std::vector<glm::vec3> &positions,
                                 const std::vector<glm::vec3> &normals,
                                 const std::vector<glm::vec2> &text_coords,
                                 const std::vector<unsigned int> &indices)
{
    // Create the VAO
    GPUBuffers buffers;
    buffers.CreateBuffers(4);
    glBindVertexArray(buffers.m_VAO);

    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, buffers.m_VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOC::POS);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOC::POS, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.m_VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOC::NORMAL);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOC::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.m_VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(text_coords[0]) * text_coords.size(), &text_coords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOC::TEX_COORD);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOC::TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.m_VBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
    CheckOpenGLError();

    return buffers;
}


GPUBuffers gpu_utils::UploadData(const std::vector<VertexFormat> &vertices,
                                 const std::vector<unsigned int>& indices)
    {
        // Create the VAO
        GPUBuffers buffers;
        buffers.CreateBuffers(2);
        glBindVertexArray(buffers.m_VAO);

        // Generate and populate the buffers with vertex attributes and the indices
        glBindBuffer(GL_ARRAY_BUFFER, buffers.m_VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.m_VBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

        // Make sure the VAO is not changed from the outside
        glBindVertexArray(0);
        CheckOpenGLError();

        return buffers;
    }
