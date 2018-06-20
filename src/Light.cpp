#include "Light.h"

Light::Light () {
}

Light::Light (glm::vec3 p, glm::vec3 c, glm::vec3 a)
{
	pos = p;
	color = c;
	atten = a;
}

void Light::SetShaderLight (GLuint shader, int index)
{
	glUseProgram (shader);
	char buffer[80];
	sprintf (buffer, "Light[%d].Position", index);
	fflush (stdout);
	printf (buffer);
	printf (" %f %f %f", pos.x, pos.y, pos.z);
	printf ("\n");
	glUniform3f (glGetUniformLocation (shader, buffer), pos.x, pos.y, pos.z);

	sprintf (buffer, "Light[%d].att", index);
	fflush (stdout);
	printf (buffer);
	printf (" %f %f %f", atten.x, atten.y, atten.z);
	printf ("\n");
	glUniform3f (glGetUniformLocation (shader, buffer), atten.x, atten.y, atten.z);

	sprintf (buffer, "Light[%d].Color", index);
	printf (buffer);
	printf (" %f %f %f", color.x, color.y, color.z);
	printf ("\n");
	fflush (stdout);
	glUniform3f (glGetUniformLocation (shader, buffer), color.x, color.y, color.z);

	glUseProgram (0);
}
