#pragma once

#include <GL/glew.h>

#include <string>
#include <iostream>

#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Misc.h"

class Shader
{
public:
    unsigned int id;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    void setVec3(const std::string& name, glm::vec3 value) const;
    void setVec4(const std::string& name, glm::vec4 value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;

    int getInt(const std::string& name) const;

private:
    void CheckShader(unsigned int shader, unsigned int status) const;
    GLuint CompileShader(std::string& source, unsigned int shader_type) const;
};

