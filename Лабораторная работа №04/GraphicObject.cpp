#include "GraphicObject.h";

GraphicObject::GraphicObject()
{
	this->angle = 0;
	this->position = { 0,0,0 };
	this->color = glm::vec4{ 1,0,0,1 };
	this->modelMatrix= glm::mat4({ 1,0,0,0 }, { 0,1,0,0 }, { 0,0,1,0 }, { this->position,1 });
}
glm::vec4& GraphicObject::getColor() 
{
	return this->color;
}
glm::mat4& GraphicObject::getModelMatrix() 
{
	return this->modelMatrix;
}

glm::vec3& GraphicObject::getPosition()
{
	return this->position;
}

float GraphicObject::getAngle()
{
	return this->angle;
}

void GraphicObject::setColor(glm::vec4 color) 
{
	this->color = color;
}

void GraphicObject::setPosition(glm::vec3 position) 
{
	this->position = position;
	this->modelMatrix[3] = { this->position,1 };
}

void GraphicObject::setAngle(float degree) 
{
	this->angle = degree;
	this->modelMatrix = glm::rotate(this->modelMatrix, this->angle, glm::vec3(0.0f, 1.0f, 0.0f));
}