#pragma once

#include "utils/glm_utils.h"


struct VertexFormat
{
    VertexFormat(glm::vec3 position,
        glm::vec3 color = glm::vec3(1),
        glm::vec3 normal = glm::vec3(0, 1, 0),
        glm::vec2 text_coord = glm::vec2(0))
        : position(position), normal(normal), text_coord(text_coord), color(color) { }

    // Position of the vertex
    glm::vec3 position;

    // Vertex normal
    glm::vec3 normal;

    // Vertex texture coordinates
    glm::vec2 text_coord;

    // Vertex color
    glm::vec3 color;
};
