#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform vec4 color;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

out vec2 texCoord;

void main()
{
   texCoord = vec2(vTexCoord.s, vTexCoord.t);
   gl_Position = projectionMatrix * modelViewMatrix * vec4(vPosition,1);
}