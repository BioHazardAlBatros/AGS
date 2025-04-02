#version 330 core

uniform	sampler2D texture_0;

in vec2 texCoord;

out vec4 outputColor;

void main (void)
{
	outputColor = texture(texture_0, texCoord);
}
