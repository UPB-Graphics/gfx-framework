/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "components/text_renderer.h"

#include <iostream>

#include "utils/text_utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "core/managers/resource_path.h"

#include "ft2build.h"
#include FT_FREETYPE_H


gfxc::TextRenderer::TextRenderer(const std::string &selfDir, GLuint width, GLuint height)
{
    // Load and configure shader
    Shader *shader = new Shader("ShaderText");
    shader->AddShader(PATH_JOIN(selfDir, RESOURCE_PATH::SHADERS, "Text.VS.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(selfDir, RESOURCE_PATH::SHADERS, "Text.FS.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    this->m_textShader = shader;

    
    int loc_projection_matrix = glGetUniformLocation(shader->program, "projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f)));

    int loc_text = glGetUniformLocation(shader->program, "text");
    glUniform1i(loc_text, 0);

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void gfxc::TextRenderer::Load(std::string font, GLuint fontSize)
{
    // First clear the previously loaded Characters
    this->Characters.clear();

    // Initialize and load the freetype library. All freetype functions
    // return a value different than 0 whenever an error occurs.
    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Then for the first 128 ASCII characters, pre-load/compile their characters and store them
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x
        };

        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    // Destroy freetype once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}


void gfxc::TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state    
    if (this->m_textShader)
    {
        glUseProgram(this->m_textShader->program);
        CheckOpenGLError();
    }

    // TODO(developer): Update this class
    int loc_text_color = glGetUniformLocation(this->m_textShader->program, "textColor");
    glUniform3f(loc_text_color, color.r, color.g, color.b);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    // Iterate through all characters
    for (auto c = text.cbegin(); c != text.cend(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 },
            { xpos,     ypos,       0.0, 0.0 },

            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 }
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);

        // Now advance cursors for next glyph. Bitshift by 6
        // to get value in pixels.
        x += (ch.Advance >> 6) * scale; 
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
