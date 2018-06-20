#version 430 core

uniform sampler2D textureSampler;
uniform vec3 lightDir;
uniform vec3 viewDir;
const float LOG2 = 1.442695;

vec4 fogColor = vec4(0,0,0.54,1);
float fogDensity = 0.5;

struct LightSource
{
        vec3 Position;
        vec3 att;
        vec3 Color;
};

uniform LightSource Light[10];

in vec3 interpNormal;
in vec2 interpTexCoord;
in vec3 vertPos;

float lDist;
float atten;

vec3 CalcPointLight(LightSource light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lDir = normalize(light.Position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5);
    // attenuation
    lDist = length(light.Position - vertPos);
    float attenuation = 1.0 / (light.att[0] + light.att[1] * lDist + light.att[2] * (lDist * lDist));    
    // combine results
    vec3 ambient  = vec3(1,1,1);
    vec3 diffuse  = light.Color * diff;
    vec3 specular = vec3(0.5,0.5,0.5) * spec;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
   // return (ambient + diffuse);
    return (ambient + diffuse + specular);
} 

void main()
{
	vec2 modifiedTexCoord = vec2(interpTexCoord.x, 1.0 - interpTexCoord.y); // Poprawka dla tekstur Ziemi, ktore bez tego wyswietlaja sie 'do gory nogami'
	vec3 color = texture2D(textureSampler, modifiedTexCoord).rgb;
	vec3 normal = normalize(interpNormal);
	// float diffuse = max(dot(normal, -lightDir), 0.0);
	// diffuse *= atten;

	float z = gl_FragCoord.z / gl_FragCoord.w / 40.0;
	float fogFactor = exp2( -fogDensity * 
				   fogDensity * 
				   z * 
				   z * 
				   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	// vec3 finCol = vec3(color * diffuse);
	// vec3 lCol = vec3(0,0,0);
	vec3 lSum = vec3(0,0,0);
	for (int i=0; i<10; i++){
		LightSource light = Light[i];
		// lDist = length(light.Position - vertPos);
		// atten = 1.0 / (light.att[0] + light.att[1] * lDist + light.att[2] * (lDist * lDist));
		// lCol = vec3(atten,atten,atten);
		// lCol *= light.Color;
		// finCol += lCol;
		vec3 res = CalcPointLight(light,normal,vertPos,-viewDir) * light.Color;
		color += res;
		lSum += res;
	}
	
	vec4 final = vec4(color, 1.0);
	//vec4 final = vec4(lSum, 1.0);
	
	//gl_FragColor = vec4(atten,atten,atten,1)*lDif;
	gl_FragColor = mix(fogColor, final, fogFactor );
	//gl_FragColor = vec4(color * diffuse, 1.0);
}
