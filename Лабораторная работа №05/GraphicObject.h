#pragma once
#include "Data.h"

class GraphicObject
{
private:
	float angle;
	glm::vec3 position;
	glm::vec4 color;
	glm::mat4 modelMatrix;

	int meshId;
	int textureId;
public:
	GraphicObject();
	glm::vec4& getColor();
	glm::mat4& getModelMatrix();
	glm::vec3& getPosition();

	float getAngle();
	int getMeshId();
	int getTextureId();

	void setColor(glm::vec4 color);
	void setPosition(glm::vec3 position);
	void setAngle(float degree);
	void setMeshId(int newId);
	void setTextureId(int newId);
};