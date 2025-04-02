#include "GraphicObject.h"

GraphicObject::GraphicObject()
{
	this->angle = 0;
	this->position = { 0,0,0 };
	this->color = glm::vec4{ 1,0,0,1 };
	this->modelMatrix= glm::mat4({ 1,0,0,0 }, { 0,1,0,0 }, { 0,0,1,0 }, { this->position,1 });
	this->dimensions = { 0,0,0 };
	this->type = GraphicObjectType::none;
	this->meshId = 0;
	this->materialId = 0;
	this->textureId = 0;
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

glm::vec3 GraphicObject::getDimensions()
{
	return this->dimensions;
}

GraphicObjectType GraphicObject::getType()
{
	return this->type;
}

int GraphicObject::getMeshId()
{
	return this->meshId;
}

int GraphicObject::getMaterialId()
{
	return this->materialId;
}

int GraphicObject::getTextureId()
{
	return this->textureId;
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

void GraphicObject::setDimensions(glm::vec3 dimensions)
{
	this->dimensions = dimensions;
}

void GraphicObject::setAngle(float degree) 
{
	this->angle = glm::radians(-degree);
	this->modelMatrix = glm::rotate(this->modelMatrix, this->angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void GraphicObject::setType(GraphicObjectType type)
{
	this->type = type;
}

void GraphicObject::setMeshId(int newId)
{
	this->meshId = newId;
}

void GraphicObject::setMaterialId(int newId)
{
	this->materialId = newId;
}

void GraphicObject::setTextureId(int newId)
{
	this->textureId = newId;
}