#pragma once
#include "Data.h"

class Light
{
	glm::vec4 direction;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

public:
	Light();

	glm::vec4& getDirection();
	glm::vec4& getAmbient();
	glm::vec4& getDiffuse();
	glm::vec4& getSpecular();

	void setDirection(glm::vec3& direction);
	void setAmbient(glm::vec4& ambient);
	void setDiffuse(glm::vec4& diffuse);
	void setSpecular(glm::vec4& specular);

};