#version 330 core

uniform vec4 color;
uniform sampler2D texture_0;

in vec2 texCoord;

out vec4 outputColor;

void main()
{
	outputColor = texture(texture_0, texCoord); 
}