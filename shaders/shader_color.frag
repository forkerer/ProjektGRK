#version 430 core

uniform vec3 objectColor;
uniform vec3 lightDir;

const float LOG2 = 1.442695;

vec4 fogColor = vec4(0,0,0.54,1);
float fogDensity = 0.0;

in vec3 interpNormal;

void main()
{
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	float z = gl_FragCoord.z / gl_FragCoord.w / 20.0;
	float fogFactor = exp2( -fogDensity * 
				   fogDensity * 
				   z * 
				   z * 
				   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	vec4 final = vec4(objectColor * diffuse, 1.0);
	gl_FragColor = mix(fogColor, final, fogFactor );
}
