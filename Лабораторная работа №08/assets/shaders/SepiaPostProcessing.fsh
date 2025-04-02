#version 330 core

uniform	sampler2D texture_0;

in vec2 texCoord;

out vec4 outputColor;

void main (void)
{
	vec4 texColor = texture(texture_0, texCoord);
	outputColor.r= 0.393 * texColor.r + 0.769 * texColor.g + 0.189 * texColor.b; 
	outputColor.g = 0.349 * texColor.r + 0.686 * texColor.g + 0.168 * texColor.b; 
	outputColor.b = 0.272 * texColor.r + 0.534 * texColor.g + 0.131 * texColor.b; 
	outputColor.a = 1.0;
}
