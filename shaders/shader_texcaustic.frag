#version 430 core

uniform sampler2D textureSampler;
uniform vec3 lightDir;
const float LOG2 = 1.442695;

vec4 fogColor = vec4(0,0,0.54,1);
float fogDensity = 0.5;

in vec3 interpNormal;
in vec2 interpTexCoord;

void main()
{
	vec2 modifiedTexCoord = vec2(interpTexCoord.x, 1.0 - interpTexCoord.y); // Poprawka dla tekstur Ziemi, ktore bez tego wyswietlaja sie 'do gory nogami'
	vec3 color = texture2D(textureSampler, modifiedTexCoord).rgb;
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0)*0.5;

	float z = gl_FragCoord.z / gl_FragCoord.w / 40.0;
	float fogFactor = exp2( -fogDensity * 
				   fogDensity * 
				   z * 
				   z * 
				   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	vec4 final = vec4(color * diffuse, 1.0);
	gl_FragColor = mix(fogColor, final, fogFactor );
	//gl_FragColor = vec4(color * diffuse, 1.0);
}
