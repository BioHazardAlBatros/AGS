#version 330 core

uniform vec2 offset;

in vec2 vPos;
out vec2 pos;

void main()
{
	pos = vPos;
	//+offset
	gl_Position = vec4(vPos+offset,0,1);
}