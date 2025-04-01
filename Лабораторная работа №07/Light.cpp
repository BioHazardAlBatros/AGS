#include "Light.h"

Light::Light() 
{
	this->diffuse = { 0.8, 0.8, 1.0, 1.0 };
	this->ambient = { 0.3,0.3,0.3,1.0 };
	//this->diffuse = { 1.0,1.0,1.0,1.0 };
	this->specular = { 0.75,0.75,0.75,1.0 };
	this->direction = { 0.4,0.5,0.6,0.0 };
}

glm::vec4& Light::getDirection() 
{
	return this->direction;
}
glm::vec4& Light::getAmbient() 
{
	return this->ambient;
}
glm::vec4& Light::getDiffuse()
{
	return this->diffuse;
}
glm::vec4& Light::getSpecular()
{
	return this->specular;
}

void Light::setDirection(glm::vec3& direction) {}
void Light::setAmbient(glm::vec4& ambient) {}
void Light::setDiffuse(glm::vec4& diffuse) {}
void Light::setSpecular(glm::vec4& specular) {}