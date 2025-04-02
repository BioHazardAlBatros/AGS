#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix[20];

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

void main ()
{
	position = vec3(modelViewMatrix[gl_InstanceID] * vec4 (vPosition, 1));
	normal = vec3(modelViewMatrix[gl_InstanceID] * vec4 (vNormal, 0));
	texCoord = vec2(vTexCoord.s, vTexCoord.t); 

	gl_Position = projectionMatrix * modelViewMatrix[gl_InstanceID] * vec4 (vPosition, 1) ;
}

