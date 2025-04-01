#pragma once
#include "Data.h"

class Material
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	GLfloat shininess;

public:
	Material();
	bool load(std::string sourceFile);
	glm::vec4& getAmbient();
	glm::vec4& getDiffuse();
	glm::vec4& getSpecular();
	float getShininess();
};