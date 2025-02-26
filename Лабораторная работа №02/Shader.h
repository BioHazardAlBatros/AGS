#pragma once

#include "Data.h"

class Shader 
{
private:	
	GLuint program;
	static GLuint currentProgram;
	std::map<std::string, GLuint> uniforms;

	GLuint getUniformLocation(std::string name);
	GLuint createShaderObject(GLenum type, std::string sourcePath);
public:
   void activate();
   static void deactivate();
   bool	load(std::string vertexShaderName,std::string fragmentShaderName);

   void setUniform(std::string name, int value);
   void setUniform(std::string name, float value);
   void setUniform(std::string name, glm::vec2);
   void setUniform(std::string name, glm::vec4);
   void setUniform(std::string name, glm::mat4);
};