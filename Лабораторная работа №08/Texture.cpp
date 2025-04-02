#include "Texture.h"

bool Texture::load(std::string sourceFile)
{
	wchar_t unicodeStr[256];
	ILuint imageID = ilGenImage();
	ilBindImage(imageID);

	wsprintf(unicodeStr, L"%S", sourceFile.c_str());
	if (IL_FALSE == ilLoadImage(unicodeStr))
		return false;

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	int format = ilGetInteger(IL_IMAGE_FORMAT);
	int type = ilGetInteger(IL_IMAGE_TYPE);

	std::vector<BYTE> pixmap(width * height * 4);

	ilCopyPixels(0, 0, 0, width, height, 1, format, type, pixmap.data());
	ilBindImage(0);
	ilDeleteImage(imageID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &this->texIndex);
	glBindTexture(GL_TEXTURE_2D, this->texIndex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, type, pixmap.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	GLfloat anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

	printf("%d bytes of texture data loaded.\n", width*height*4);
	return true;
}
void Texture::bind(GLenum texUnit) 
{
	glEnable(texUnit);
	glBindTexture(GL_TEXTURE_2D, this->texIndex);
}