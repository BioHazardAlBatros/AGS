#version 330 core

uniform vec4 color1;
uniform vec4 color2;

in vec2 pos;
out vec4 fragColor;

void main()
{
// x2 = 0, y2 = 0.5  ; x1=0.5 y1=0;
	//fragColor = color1*((pos.x * (-0.5) - (pos.y - 0.5) * 0.5))+color2*0.25;

	fragColor = int(pos.y<pos.x)*color2 + int(pos.y>pos.x)*color1;
//	fragColor = vec4(pos.x+0.5,pos.y+0.5,0.0,0.0);
//	fragColor = ((pos.x*pos.x+pos.y*pos.y))*color1;
//	fragColor = color2*(pos.x+0.5)+color1*(pos.y+0.5);
}