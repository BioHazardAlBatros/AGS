#pragma once

#include "Data.h"

class Shader 
{
private:
	GLuint createShaderObject(GLenum type, std::string sourcePath);
	GLuint program;
public:
	void activate();
	static void deactivate();
	bool load(std::string vertexShaderName,std::string fragmentShaderName);
};