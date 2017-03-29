#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "Shader.h"
#include "Model.h"
#include "FPSCamera.h"
#include "MathHelper.h"
#include "Light.h"	
#include "teapot.h"

Model* teapotModel;
Model* planeModel;

FPSCamera* camera;
Light*	light;					

GLfloat* rotXMatrix;
GLfloat* rotYMatrix;
GLfloat* rotZMatrix;
GLfloat* transMatrix;

GLfloat* scaleMatrix;
GLfloat* tempMatrix;
GLfloat* tempMatrix2;

GLfloat zVel, xVel, maxSpeed;


//============= NEW =================
GLfloat fov;

//===================================

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Do everything related to the teapot
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	MathHelper::makeScale(scaleMatrix, 0.1f, 0.1f, 0.1f);
	MathHelper::makeRotateY(rotYMatrix, 1.57f);
	MathHelper::matrixMult4x4(tempMatrix, scaleMatrix, rotYMatrix);
	MathHelper::makeTranslate(tempMatrix2, 0, -2, -3.0f);
	MathHelper::matrixMult4x4(teapotModel->mM, tempMatrix2, tempMatrix);

	// Sets the cameras VP matrices for this shader 
	camera->setVPInShader(teapotModel->shader->programID);
	// Sets the light position for the teapot model's shader. We don't need to do that for the plane
	light->setLightPositionInShader(teapotModel->shader->programID);
	teapotModel->render();

	glutSwapBuffers();
	glutPostRedisplay();
}

// Any time the window is resized, this function gets called.  It's setup to the 
// "glutReshapeFunc" in main.
void changeViewport(int w, int h){
	glViewport(0, 0, w, h);
}

int main (int argc, char** argv) {
	// Initialize GLUT
	glutInit(&argc, argv);

	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(800, 800);
	// Create the window with the title "Triangle"
	glutCreateWindow("Texture Lab");
	// Bind the two functions (above) to respond when necessary
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	
	
	// Remember, we always init GLEW after we create a window...
	// This asks the GPU driver for all of those gl functions
	if (GLEW_OK != glewInit()) {
		exit(1);
	}
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable (GL_DEPTH_TEST);
	
	glEnable (GL_CULL_FACE);
	// These two commands aren't necessary, but I thought I'd throw them in
	// so you could see the defaults.
	glCullFace(GL_BACK);
	glFrontFace (GL_CCW);

	const char* vertexProgram = "../ShaderLab/TextureShader.vsh";
	const char* fragmentProgram = "../ShaderLab/TextureShader.fsh";
	Shader* myShader = new Shader(vertexProgram, fragmentProgram);

	vertexProgram = "../ShaderLab/SolidShader.vsh";
	fragmentProgram = "../ShaderLab/SolidShader.fsh";
	Shader* solidShader = new Shader(vertexProgram, fragmentProgram);
	
	// This is a directional light!
	GLfloat lightPos[] = {10.0f, 0.0f, 10.0f, 0.0f};		
	GLfloat lightAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightDiffuse[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat lightSpecular[] = {0.9f, 0.9f, 0.9f, 1.0f};

	GLfloat matAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat matDiffuse[] = {0.7f, 0.1f, 0.1f, 1.0f};
	GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat shininess = 100.0f;

	light = new Light(lightPos, lightAmbient, lightDiffuse, lightSpecular);
	//========================================================
	
	teapotModel = new Model(myShader);
	teapotModel->DEBUG = false;
	teapotModel->setGeometry(teapot_vertices, num_vertices * 3 * sizeof(GLfloat));
	teapotModel->setNormals(teapot_normals, num_normals * 3 * sizeof(GLfloat));
	teapotModel->setTextureCoordinates(teapot_uvs, num_vertices * 2 * sizeof(GLfloat));
	teapotModel->setIndexBuffer(teapot_indices, num_indices*sizeof(GLint));
	teapotModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	teapotModel->setLight(light);
	teapotModel->loadBitmap("../ShaderLab/teapot.bmp");
	teapotModel->loadVBO();
	


	// Set up the camera 
	fov = 60.0f;
	camera = new FPSCamera(fov, 1.0f, 1.0f, 1000.0f, false);
	MathHelper::makeTranslate(camera->mV, 0.0f, 0, 0.0f);

	// Allocate memory for the matrices
	rotXMatrix = new GLfloat[16];
	rotYMatrix = new GLfloat[16];
	rotZMatrix = new GLfloat[16];
	transMatrix = new GLfloat[16];
	scaleMatrix = new GLfloat[16];
	tempMatrix = new GLfloat[16];
	tempMatrix2 = new GLfloat[16];

	glutMainLoop();

	return 0;
}