#include "core/gpu/shader.h"

#include <fstream>
#include <iostream>


Shader::Shader(const std::string &name)
{
    program = 0;
    shaderName = name;
    shaderFiles.reserve(5);
}


Shader::~Shader()
{
    glDeleteProgram(program);
}


const char * Shader::GetName() const
{
    return shaderName.c_str();
}


GLuint Shader::GetProgramID() const
{
    return program;
}


void Shader::Use() const
{
    if (program)
    {
        glUseProgram(program);
        CheckOpenGLError();
    }
}


unsigned int Shader::Reload()
{
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }

    return CreateAndLink();
}


void Shader::BindTexturesUnits()
{
    for (int i = 0; i < MAX_2D_TEXTURES; i++) {
        if (loc_textures[i] >= 0)
            glUniform1i(loc_textures[i], i);
    }
}


GLint Shader::GetUniformLocation(const char *uniformName) const
{
    return glGetUniformLocation(program, uniformName);
}


void Shader::OnLoad(std::function<void()> onLoad)
{
    loadObservers.push_back(onLoad);
}


void Shader::GetUniforms()
{
    // MVP
    loc_model_matrix        = GetUniformLocation("Model");
    loc_view_matrix         = GetUniformLocation("View");
    loc_projection_matrix   = GetUniformLocation("Projection");

    // Lighting and shadows
    loc_light_pos           = GetUniformLocation("light_position");
    loc_light_color         = GetUniformLocation("light_color");
    loc_light_radius        = GetUniformLocation("light_radius");
    loc_light_direction     = GetUniformLocation("light_direction");

    // Camera
    loc_eye_pos             = GetUniformLocation("eye_position");
    loc_eye_forward         = GetUniformLocation("eye_forward");
    loc_z_far               = GetUniformLocation("zFar");
    loc_z_near              = GetUniformLocation("zNear");

    // General
    loc_resolution          = GetUniformLocation("resolution");

    char buffer[64];

    // Textures
    for (int i = 0; i < MAX_2D_TEXTURES; i++) {
        sprintf(buffer, "u_texture_%d", i);
        loc_textures[i]     = GetUniformLocation(buffer);
    }

    // Text
    text_color              = GetUniformLocation("text_color");

    BindTexturesUnits();

    CheckOpenGLError();
}


void Shader::AddShader(const std::string & shaderFile, GLenum shaderType)
{
    ShaderFile S;
    S.file = shaderFile;
    S.type = shaderType;
    shaderFiles.push_back(S);
}


void Shader::AddShaderCode(const std::string &shaderCode, GLenum shaderType)
{
    ShaderFile S;
    S.file = shaderCode;
    S.type = shaderType;
    shaderCodes.push_back(S);
}


unsigned int Shader::CreateAndLink()
{
    std::vector<unsigned int> shaders;

    // Compile shaders
    for (auto S : shaderFiles) {
        auto shaderID = Shader::CreateShader(S.file, S.type);
        if (shaderID) {
            shaders.push_back(shaderID);
        } else {
            return 0;
        }
    }

    for (auto S : shaderCodes)
    {
        auto shaderID = Shader::CompileShader(S.file, S.type);
        if (shaderID) {
            shaders.push_back(shaderID);
        } else {
            return 0;
        }
    }

    // Create Program and Link
    if (shaders.size()) {
        program = Shader::CreateProgram(shaders);

        if (program)
        {
            glUseProgram(program);
            GetUniforms();
            for (auto Observer : loadObservers) {
                Observer();
            }
            return program;
        }
    }
    return 0;
}


void Shader::ClearShaders()
{
    shaderFiles.clear();
}


static std::string InjectDefines(const std::string &shaderCode)
{
    std::string defines;
    size_t pos = shaderCode.find_first_of("\n");

#ifdef SOLVED
    defines += "\n#define SOLVED";
#endif

    if (pos == std::string::npos)
    {
        return shaderCode + defines;
    }

    return shaderCode.substr(0, pos) + defines + shaderCode.substr(pos, std::string::npos);
}


unsigned int Shader::CreateShader(const std::string &shaderFile, GLenum shaderType)
{
    std::string shader_code;
    std::ifstream file(shaderFile.c_str(), std::ios::in);

    if (!file.good()) {
        std::cout << "\tCould not open file: " << shaderFile << std::endl;
        std::terminate();
    }

    std::cout << "\tFILE = " << shaderFile;

    // Get file content
    file.seekg(0, std::ios::end);
    shader_code.resize((unsigned int)file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&shader_code[0], shader_code.size());
    file.close();

    return CompileShader(InjectDefines(shader_code), shaderType);
}


unsigned int Shader::CompileShader(const std::string shaderCode, GLenum shaderType)
{
    int infoLogLength = 0;
    int compileResult = 0;
    unsigned int glShaderObject;

    // Create new shader object
    glShaderObject = glCreateShader(shaderType);
    if (glShaderObject == 0) {
        std::cout << "\t ..... ERROR " << std::endl;
        return 0;
    }

    const char *shader_code_ptr = shaderCode.c_str();
    const int shader_code_size = (int)shaderCode.size();

    glShaderSource(glShaderObject, 1, &shader_code_ptr, &shader_code_size);
    glCompileShader(glShaderObject);
    glGetShaderiv(glShaderObject, GL_COMPILE_STATUS, &compileResult);

    // LOG COMPILE ERRORS
    if (compileResult == GL_FALSE)
    {
        std::string str_shader_type = "";

        if (shaderType == GL_VERTEX_SHADER)              str_shader_type="VERTEX";
#ifndef OPENGL_ES
        if (shaderType == GL_TESS_CONTROL_SHADER)        str_shader_type="TESS CONTROL";
        if (shaderType == GL_TESS_EVALUATION_SHADER)     str_shader_type="TESS EVALUATION";
        if (shaderType == GL_GEOMETRY_SHADER)            str_shader_type="GEOMETRY";
#endif
        if (shaderType == GL_FRAGMENT_SHADER)            str_shader_type="FRAGMENT";
        if (shaderType == GL_COMPUTE_SHADER)             str_shader_type="COMPUTE";

        glGetShaderiv(glShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> shader_log(infoLogLength);
        glGetShaderInfoLog(glShaderObject, infoLogLength, NULL, &shader_log[0]);

        std::cout << "\n-----------------------------------------------------\n";
        std::cout << "\n[ERROR]: [" << str_shader_type << " SHADER]\n\n";
        std::cout << &shader_log[0] << "\n";
        std::cout << "-----------------------------------------------------" << std::endl;

        return 0;
    }

    std::cout << "\t ..... COMPILED " << std::endl;

    return glShaderObject;
}


unsigned int Shader::CreateProgram(const std::vector<unsigned int> &shaderObjects)
{
    int infoLogLength = 0;
    int linkResult = 0;

    // build OpenGL program object and link all the OpenGL shader objects
    unsigned int glProgramObject = glCreateProgram();

    for (auto shader : shaderObjects)
        glAttachShader(glProgramObject, shader);

    glLinkProgram(glProgramObject);
    glGetProgramiv(glProgramObject, GL_LINK_STATUS, &linkResult);

    // LOG LINK ERRORS
    if (linkResult == GL_FALSE) {
        glGetProgramiv(glProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> program_log(infoLogLength);
        glGetProgramInfoLog(glProgramObject, infoLogLength, NULL, &program_log[0]);

        std::cout << "Shader Loader : LINK ERROR" << std::endl;
        std::cout << &program_log[0] << std::endl;

        return 0;
    }

    // Delete the shader objects because we do not need them any more
    for (auto shader : shaderObjects)
        glDeleteShader(shader);

    CheckOpenGLError();

    return glProgramObject;
}
