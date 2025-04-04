#pragma once
#include "Data.h"

class Camera
{
public:
	Camera();
	void setProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	glm::mat4& getProjectionMatrix();
	glm::mat4& getViewMatrix();
	glm::vec3& getPosition();
	void moveOXZ(float dx, float dz);
	void rotate(float horizAngle, float vertAngle);
	void zoom(float dR);
	void lookAt(glm::vec3 target);

private:
	void recalculateViewMatrix();

	float fov;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 up;
	glm::vec3 sideDirection;
	glm::vec3 fwdDirection;
};