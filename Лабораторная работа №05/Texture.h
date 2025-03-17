#pragma once
#include "Data.h"

class Texture
{
	GLuint texIndex;
public:
	bool load(std::string sourceFile);
	void bind(GLenum texUnit = GL_TEXTURE0);
};