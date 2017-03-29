#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "Shader.h"
#include "Model.h"
#include "FPSCamera.h"
#include "MathHelper.h"
#include "Light.h"				// NEW!!
#include "buddha.h"

Model* myModel;
FPSCamera* camera;
Light*	light;					// NEW!!

GLfloat* rotXMatrix;
GLfloat* rotYMatrix;
GLfloat* rotZMatrix;
GLfloat* transMatrix;

GLfloat* scaleMatrix;
GLfloat* tempMatrix;
GLfloat  theta;
GLfloat  scaleAmount;

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	theta += 0.04f;
	MathHelper::makeScale(scaleMatrix, 0.10f, 0.10f, 0.10f); // Create a scale matrix for Buddha

	MathHelper::makeRotateY(myModel->mR, theta);			// NEW
	MathHelper::makeTranslate(transMatrix, 0, -0.5, -1);
	MathHelper::makeRotateY(rotYMatrix, theta);

	MathHelper::matrixMult4x4(tempMatrix, scaleMatrix, rotYMatrix);
	// The order here is important!
	MathHelper::matrixMult4x4(myModel->mM, transMatrix, tempMatrix);

	// We'll only set the View and Perspective matrix when/if we change shaders
	camera->setVPInShader(myModel->shader->programID);
	light->setLightPositionInShader();							// NEW!!!
	myModel->render();

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
	glutInitWindowSize(800, 600);
	// Create the window with the title "Lighting Lab"
	glutCreateWindow("Lighting Lab");
	// Bind the two functions (above) to respond when necessary
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	
	// Remember, we always init GLEW after we create a window...
	// This asks the GPU driver for all of those gl functions
	if (GLEW_OK != glewInit()) {
		exit(1);
	}
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	

	const char* vertexProgram = "../ShaderLab/vertexProgram.vsh";
	const char* fragmentProgram = "../ShaderLab/fragmentProgram.fsh";
	Shader* myShader = new Shader(vertexProgram, fragmentProgram);
	
	//=========================  NEW!!  =====================
	// This is a directional light!
	GLfloat lightPos[] = {0.0f, 0.0f, 10.0f, 0.0f};		
	GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat lightDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat lightSpecular[] = {0.90f, 0.9f, 0.3f, 1.0f};

	GLfloat matAmbient[] = {0.8f, 0.5f, 0.0f, 1.0f};
	GLfloat matDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat shininess = 250.0f;

	light = new Light(lightPos, lightAmbient, lightDiffuse, lightSpecular, myShader);
	
	//========================================================
	
	myModel = new Model(myShader);
	myModel->setGeometry(vertices, num_vertices*sizeof(GL_FLOAT)*3);			// From buddha.h
	myModel->setIndexBuffer(indices, num_indices*sizeof(GLuint));
	myModel->setNormals(normals, num_normals*sizeof(GL_FLOAT)*3);
	myModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		// NEW!!
	myModel->setLight(light);													// NEW!!
	myModel->loadVBO();

	// Set up the camera 
	camera = new FPSCamera(60.0f, 1.0f, 1.0f, 1000.0f);
	// Set up some matrices for the model
	theta = 0.0f;
	scaleAmount = myModel->scale;

	// Allocate memory for the matrices
	rotXMatrix = new GLfloat[16];
	rotYMatrix = new GLfloat[16];
	rotZMatrix = new GLfloat[16];
	transMatrix = new GLfloat[16];
	scaleMatrix = new GLfloat[16];
	tempMatrix = new GLfloat[16];

	glutMainLoop();

	return 0;
}