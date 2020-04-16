#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

static const char* GetGLErrorStr(GLenum err)
{
	switch (err)
	{
	case GL_NO_ERROR:          return "No error";
	case GL_INVALID_ENUM:      return "Invalid enum";
	case GL_INVALID_VALUE:     return "Invalid value";
	case GL_INVALID_OPERATION: return "Invalid operation";
	case GL_STACK_OVERFLOW:    return "Stack overflow";
	case GL_STACK_UNDERFLOW:   return "Stack underflow";
	case GL_OUT_OF_MEMORY:     return "Out of memory";
	default:                   return "Unknown error";
	}
}

static void CheckGLError()
{
	while (true)
	{
		const GLenum err = glGetError();
		if (GL_NO_ERROR == err)
			break;

		std::cout << "GL Error: " << GetGLErrorStr(err) << std::endl;

	}
}

static std::string Loadfile(const char* path) {
	try {
		auto file = std::ifstream();
		auto fileContent = std::stringstream();

		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(path);

		fileContent << file.rdbuf();

		file.close();

		return fileContent.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "ERROR: File could not be read.\n\n" << e.what() << std::endl;
	}

	return "";
}