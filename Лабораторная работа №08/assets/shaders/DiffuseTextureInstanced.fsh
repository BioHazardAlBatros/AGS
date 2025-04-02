#version 330 core

uniform sampler2D texture_0;

uniform vec4 mAmbient;
uniform vec4 mDiffuse;
uniform vec4 mSpecular;
uniform float mShininess;

uniform vec4 lAmbient;
uniform vec4 lDiffuse;
uniform vec4 lSpecular;
uniform vec4 lPosition;

uniform vec4 fogColor;
uniform float fogStartDistance;
uniform float fogEndDistance;
uniform float fogDensity;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 outputColor;

void main (void)
{
	vec3 n_Normal = normalize(normal);
	vec3 n_ToEye = normalize (vec3(0,0,0) - position);
	vec3 n_ToLight = normalize(vec3(lPosition));
	vec3 n_Reflect = normalize(reflect(-n_ToLight,n_Normal));
	
	vec4 ambient = mAmbient * lAmbient;  
	vec4 diffuse = mDiffuse * lDiffuse * max(dot(n_Normal, n_ToLight), 0.0f);
	vec4 specular = mSpecular * lSpecular * pow(max(dot(n_ToEye, n_Reflect), 0.0f), mShininess);

 	vec3 color = vec3(ambient + diffuse + specular); 
	float alpha = mDiffuse.a;

	vec4 texColor = texture(texture_0, texCoord);
	color = color * vec3(texColor);	
	
	float fragmentDistance = length(position);
	float A = exp(-fogDensity * clamp((fragmentDistance -fogStartDistance) / (fogEndDistance - fogStartDistance), 0.0, 1.0));

	outputColor = mix(fogColor,vec4(color, alpha),A);
}

