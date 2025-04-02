#version 330 core

layout(location = 0) in vec2 vPosition;

out vec2 texCoord;

void main ()
{
	texCoord = vPosition + vec2(0.5f, 0.5f); 
	gl_Position = vec4(vPosition * 2.0, 0, 1);
}