#version 330 core

uniform	sampler2D texture_0;

in vec2 texCoord;

out vec4 outputColor;

void main (void)
{
	vec4 texColor = texture(texture_0, texCoord);
	outputColor = vec4(1.5*texColor.r,0.5*texColor.g,0.3 * texColor.b, texColor.a);
}
