#include "Shader.h"


void Shader::CheckShader(unsigned int id, unsigned int status) const {

	int success = 0;
	constexpr size_t buffer_size = 512;
	char infoLog[buffer_size] = { 0 };

	switch (status) {
	case GL_COMPILE_STATUS:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(id, buffer_size, NULL, infoLog);
			std::cout << "ERROR: " << infoLog << std::endl;
		}
		break;
	case GL_LINK_STATUS:
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id, buffer_size, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		break;
	default:
		break;
	}
}

GLuint Shader::CompileShader(std::string& source, unsigned int shader_type) const {
	GLuint id = glCreateShader(shader_type);
	const GLchar* _source = source.c_str();
	glShaderSource(id, 1, &_source, 0);
	glCompileShader(id);
	CheckShader(id, GL_COMPILE_STATUS);

	return id;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	auto vertexSource = Loadfile(vertexPath);
	auto fragmentSource = Loadfile(fragmentPath);

	auto vertex = CompileShader(vertexSource, GL_VERTEX_SHADER);
	auto fragment = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
	
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	CheckShader(id, GL_LINK_STATUS);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	CheckGLError();
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, glm::vec4 value) const
{
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, false, glm::value_ptr(value));
}

int Shader::getInt(const std::string& name) const
{
	int out = 0;
	const auto loc = glGetUniformLocation(id, name.c_str());
	assert(loc != -1);
	glGetUniformiv(id, loc, &out);
	return out;
}
