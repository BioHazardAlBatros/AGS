#include "Texture.h"

bool Texture::load(std::string sourceFile)
{
	std::ifstream File(sourceFile);
	if (!File)
		return false;
	/////////////////////////////////////////////////////////
}
void Texture::bind(GLenum texUnit) 
{
	
}