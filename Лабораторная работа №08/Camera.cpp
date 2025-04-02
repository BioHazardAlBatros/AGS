#include "Camera.h"

Camera::Camera()
{
	this->position = glm::vec3({ 0.0,3.0,5.0 });
	lookAt({ 0.0,0.0,0.0 });
	this->fov = 35.0f;

	setProjectionMatrix(this->fov, 800.0 / 600.0, 1.0, 100.0);
	recalculateViewMatrix();
}

glm::vec3& Camera::getPosition()
{
	return this->position;
}

void Camera::setProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
	this->projectionMatrix = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
}

void Camera::lookAt(glm::vec3 target)
{
	glm::vec3 direction = glm::normalize(target - this->position);
	this->rotation = { glm::degrees(atan2(direction.z, direction.x)),
					  glm::degrees(asin(direction.y)),
					  0.0f };
}

glm::mat4& Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

glm::mat4& Camera::getViewMatrix()
{
	return this->viewMatrix;
}

void Camera::moveOXZ(float dx, float dz)
{
	this->position += this->fwdDirection * dx + this->sideDirection * dz;
	recalculateViewMatrix();
}

void Camera::rotate(float horizAngle, float vertAngle)
{
	static float sensitivity = 0.2f;
	this->rotation.x += horizAngle* sensitivity;
	this->rotation.y = glm::clamp(this->rotation.y+vertAngle* sensitivity, -85.0f, 85.0f);
	recalculateViewMatrix();
}
void Camera::zoom(float dR) 
{
//	this->distance = std::max(1.0f, std::min(this->distance - dR, 90.0f));
//	setProjectionMatrix(this->distance, 800.0 / 600.0, 1.0, 100.0);
	this->position += this->fwdDirection * dR;
	recalculateViewMatrix();
}
void Camera::recalculateViewMatrix() 
{
	glm::vec3 front = { cos(glm::radians(this->rotation.x)) * cos(glm::radians(this->rotation.y)),
						sin(glm::radians(this->rotation.y)),
						sin(glm::radians(this->rotation.x)) * cos(glm::radians(this->rotation.y)) };

	this->fwdDirection = glm::normalize(front);
	this->sideDirection = glm::normalize(glm::cross(this->fwdDirection, glm::vec3{0,1,0}));
	this->up = glm::normalize(glm::cross(this->sideDirection, this->fwdDirection));
	this->viewMatrix = glm::lookAt(this->position, this->position + this->fwdDirection, this->up);
}
