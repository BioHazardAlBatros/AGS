#pragma once
#include "Data.h"

class GraphicObject
{
private:
	glm::vec3 position;
	float angle;
	glm::vec4 color;
	glm::mat4 modelMatrix;

public:
	GraphicObject();
	glm::vec4& getColor();
	glm::mat4& getModelMatrix();
	glm::vec3& getPosition();
	float getAngle();

	void setColor(glm::vec4 color);
	void setPosition(glm::vec3 position);
	void setAngle(float degree);

};