#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"
#include "Placeable.h"
#include "StaticObject.h"
#include "PathFollowable.h"
#include "CameraBase.h"
#include "BasicCamera.h"
#include "FPSCamera.h"
#include "Light.h"

GLuint programColor;
GLuint programTexture;
GLuint fishTexture;
GLuint duneTexture;
GLuint groundShader;
GLuint causticShader;

GLuint caustics[32];
static GLfloat causticScale = 1.0;
static int currentCaustic = 0;
static int causticsChangeFreq = 62;
int lastCausticsChange = 0;

static GLfloat lightDiffuseColor[] = { 1.0, 1.5, 1.0, 1.0 };

Core::Shader_Loader shaderLoader;

Light lights[10];

obj::Model shipModel;
obj::Model sphereModel;
obj::Model fishModel;
obj::Model duneModel;

CameraBase *curCamera;

glm::vec3 cameraPos = glm::vec3(0, 0, 5);
glm::vec3 cameraDir; // Wektor "do przodu" kamery
glm::vec3 cameraSide; // Wektor "w bok" kamery
float cameraAngle = 0;



enum DrawPass {
	PASS_NORMAL,
	PASS_CAUSTICS
} drawPass;

void updateCamera ();
void renderScene ();
void updateFollowables ();
void drawObjects (DrawPass);

std::vector<glm::vec3> fishPath;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

struct mousePos {
	int x = 0;
	int y = 0;
} lastMousePos;

struct mouseMov {
	int x = 0;
	int y = 0;
} lastMouseMov;

glm::quat rotation = glm::quat(1, 0, 0, 0);
glm::quat zRot = glm::quat (1, 0, 0, 0);
float zRotf = 0;

std::vector<Placeable*> objects;
std::vector<PathFollowable*> followables;

GLuint textureAsteroid;

void keyboard (unsigned char key, int x, int y)
{
	return curCamera->HandleKeys (key, x, y);
}

void mouse(int x, int y)
{
	return curCamera->HandleMouse (x, y);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform3f (glGetUniformLocation (program, "viewDir"), curCamera->rotation.x, curCamera->rotation.y, curCamera->rotation.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint textureId, GLuint program)
{
	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	Core::SetActiveTexture(textureId, "textureSampler", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}
void renderScene()
{
	updateCamera ();
	updateFollowables ();

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor (0.0f, 0.0f, 0.54f, 1.0f);

	glPushMatrix ();

	drawObjects (PASS_NORMAL);

	glDepthMask (GL_FALSE);
	glDepthFunc (GL_EQUAL);
	glBlendFunc (GL_ZERO, GL_SRC_COLOR);
	glEnable (GL_BLEND);

	drawObjects (PASS_CAUSTICS);

	glDepthMask (GL_TRUE);
	glDepthFunc (GL_LESS);
	glDisable (GL_BLEND);

	glPopMatrix ();

	glutSwapBuffers();
}

void updateCamera () {
	// Aktualizacja macierzy widoku i rzutowania
	cameraMatrix = curCamera->createCameraMatrix ();
	perspectiveMatrix = Core::createPerspectiveMatrix ();
}

void updateFollowables () {
	for (PathFollowable* followable : followables) {
		followable->Update ();
	}
}

void drawObjects (DrawPass pass) {
	/* The 0.03 in the Y column is just to shift the texture coordinates
	a little based on Y (depth in the water) so that vertical faces
	(like on the cube) do not get totally vertical caustics. */
	GLfloat sPlane[4] = { 0.05, 0.03, 0.0, 0.0 };
	GLfloat tPlane[4] = { 0.0, 0.03, 0.05, 0.0 };

	/* The causticScale determines how large the caustic "ripples" will
	be.  See the "Increate/Decrease ripple size" menu options. */

	sPlane[0] = 0.05 * causticScale;
	sPlane[1] = 0.03 * causticScale;

	tPlane[1] = 0.03 * causticScale;
	tPlane[2] = 0.05 * causticScale;

	if (pass == PASS_CAUSTICS) {
		//glColor3f (1.0, 1.0, 1.0);
		glDisable (GL_LIGHTING);
		glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv (GL_S, GL_OBJECT_PLANE, sPlane);
		glTexGenfv (GL_T, GL_OBJECT_PLANE, tPlane);
		glEnable (GL_TEXTURE_GEN_S);
		glEnable (GL_TEXTURE_GEN_T);
	}

	for (Placeable* object : objects) {
		GLuint texture = object->texture;
		GLuint programTex = programTexture;
		if (pass == PASS_CAUSTICS) {
			texture = caustics[currentCaustic];
			programTex = object->secondShader;
		}
		drawObjectTexture (object->model, object->GetModelMatrix (), texture, programTex);
	}

	if (pass == PASS_CAUSTICS) {
		glEnable (GL_LIGHTING);
		glDisable (GL_TEXTURE_GEN_S);
		glDisable (GL_TEXTURE_GEN_T);
	}
}

void init()
{
	srand(time(0));
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	groundShader = shaderLoader.CreateProgram ("shaders/shader_texground.vert", "shaders/shader_texground.frag");
	causticShader = shaderLoader.CreateProgram ("shaders/shader_texcaustic.vert", "shaders/shader_texcaustic.frag");
	//sphereModel = obj::loadModelFromFile("models/sphere.obj");
	//shipModel = obj::loadModelFromFile("models/spaceship.obj");
	//textureAsteroid = Core::LoadTexture("textures/asteroid.png");
	fishTexture = Core::LoadTexture ("textures/fish_texture.png");
	fishModel = obj::loadModelFromFile ("models/fish.obj");
	duneTexture = Core::LoadTexture ("textures/ground.png");
	duneModel = obj::loadModelFromFile ("models/dune.obj");

	glLightfv (GL_LIGHT0, GL_DIFFUSE, lightDiffuseColor);
	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHTING);

	GLuint imageData;
	//glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	for (int i = 0; i < 32; i++) {
		char filename[80];

		sprintf (filename, "textures/caust%02d.png", i);
		fflush (stdout);
		imageData = Core::LoadTexture (filename);
		caustics[i] = imageData;
	}

	curCamera = new FPSCamera ();

	/*for (int i = 0; i < 5; i++) {
		StaticObject *planet =  new StaticObject (&sphereModel, textureAsteroid);
		planet->SetPosition (glm::vec3 (-5 + rand () % 10, 0, -5 + rand () % 10));
		objects.push_back (planet);
	}*/

	StaticObject *dune = new StaticObject (&duneModel, duneTexture);
	dune->SetPosition (glm::vec3 (0, -10, 0));
	dune->SetScale (glm::vec3 (100, 50, 100));
	dune->secondShader = groundShader;
	objects.push_back (dune);

	fishPath.push_back (glm::vec3 (-0.946, 0, -27.096));
	fishPath.push_back (glm::vec3 (-17.45, 1.37, -19.4));
	fishPath.push_back (glm::vec3 (-16.1, 1.4, -8.396));
	fishPath.push_back (glm::vec3 (-16.946, 1.4, -1.096));
	fishPath.push_back (glm::vec3 (-10.946, 0, 12.096));
	fishPath.push_back (glm::vec3 (0.75, -3.4, -3.096));
	fishPath.push_back (glm::vec3 (9.6, -5.5, 21.096));
	fishPath.push_back (glm::vec3 (25.46, -5.5, 24.496));
	fishPath.push_back (glm::vec3 (34.6, -7.5, 5.4));
	fishPath.push_back (glm::vec3 (30, -7.5, -0.096));
	fishPath.push_back (glm::vec3 (32.6, -3.5, -3.4));
	fishPath.push_back (glm::vec3 (33.6, 0.1, -15.6));
	fishPath.push_back (glm::vec3 (29.8, 0, -24.596));
	fishPath.push_back (glm::vec3 (21.8, 0, -26.596));
	fishPath.push_back (glm::vec3 (9.8, 0, -25.596));

	for (int i = 0; i < 5; i++) {
		PathFollowable *fish = new PathFollowable (&fishModel, fishTexture);
		fish->secondShader = causticShader;
		glm::vec3 startPos = glm::vec3 (-5 + rand () % 10, -3 + rand () % 6, -5 + rand () % 10);
		fish->SetPosition (startPos);
		fish->SetRotation (glm::vec3 (90, 0, 0));
		fish->SetPath (fishPath);
		/*fish->AddPointToPath (startPos);
		for (int j = 0; j < 9; j++) {
			glm::vec3 pos = glm::vec3 (-15 + rand () % 30, -1 + rand () % 2, -15 + rand () % 30);
			fish->AddPointToPath (pos);
		}*/
		followables.push_back (fish);
		objects.push_back (fish);
	}
	
	for (int i = 0; i < 10; i++) {
		glm::vec3 pos = glm::vec3 (-75 + rand () % 150, -5 + rand () % 10, -75 + rand () % 150);
		float col1 = ((float)(rand () % 255)) / 255;
		float col2 = ((float)(rand () % 255)) / 255;
		float col3 = ((float)(rand () % 255)) / 255;
		glm::vec3 col = glm::vec3 (col1, col2, col3);
		glm::vec3 atten = glm::vec3 (1, 0.01f, 0.0096f);
		lights[i] = Light (pos, col, atten);

		lights[i].SetShaderLight (programTexture, i);
		lights[i].SetShaderLight (groundShader, i);
	}

}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	float time = glutGet (GLUT_ELAPSED_TIME);
	if ((time - lastCausticsChange) > causticsChangeFreq) {
		lastCausticsChange = time;
		currentCaustic = (currentCaustic + 1) % 32;
	}
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("OpenGL Pierwszy Program");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
