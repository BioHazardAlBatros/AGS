#pragma once
#include "Data.h"

class FBO
{
	GLuint FBO_index;
	GLuint colorTex;
	GLuint depthTex;
	int width, height, samples;
	bool multisample;
public:
	FBO();
	void init(int w, int h, bool multisample = false);
	void bind();
	void resolveToFbo(FBO& fbo);
	void bindColorTexture(GLenum texUnit = GL_TEXTURE0);
	void bindDepthTexture(GLenum texUnit = GL_TEXTURE1);
	static void unbind();

	bool isMultisample();
//	static void ScreenshotFBO(FBO& fbo, const char* filename);
};

