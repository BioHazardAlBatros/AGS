#version 330 core

uniform	sampler2D texture_0;

in vec2 texCoord;

out vec4 outputColor;

void main (void)
{
	vec4 texColor = texture(texture_0, texCoord);
	float luminance = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
	outputColor = vec4(luminance, luminance, luminance, 1.0);
}
