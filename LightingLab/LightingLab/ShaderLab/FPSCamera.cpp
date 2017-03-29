#include "FPSCamera.h"


FPSCamera::FPSCamera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane)
{
	// The View matrix (the position and orientation of the camera)
	this->mV = new GLfloat[16];
	MathHelper::makeIdentity(mV);

	// The Perspective matrix - note that fov, aspect and the planes reside in the P matrix.
	this->mP = new GLfloat[16];
	this->setPerspective(fov, aspect, nearPlane, farPlane);

	// Some temporary matrices for multiplying
	this->tempMatrix = new GLfloat[16];
	this->tempMatrix2 = new GLfloat[16];
}
FPSCamera::~FPSCamera()
{
	delete(mP);
	delete(mV);
	delete(tempMatrix);
	delete(tempMatrix2);
}

#pragma region TRANSFORM_METHODS
void FPSCamera::rotateX(GLfloat amount)
{
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeRotateX(tempMatrix2, amount);
	MathHelper::matrixMult4x4(this->mV, tempMatrix, tempMatrix2);
}
void FPSCamera::rotateY(GLfloat amount)
{
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeRotateY(tempMatrix2, amount);
	MathHelper::matrixMult4x4(this->mV, tempMatrix, tempMatrix2);
}
void FPSCamera::rotateZ(GLfloat amount)
{
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeRotateZ(tempMatrix2, amount);
	MathHelper::matrixMult4x4(this->mV, tempMatrix, tempMatrix2);
}
void FPSCamera::moveX(GLfloat amount)
{
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeTranslate(tempMatrix2, amount, 0.0f, 0.0f);
	MathHelper::matrixMult4x4(this->mV, tempMatrix, tempMatrix2);
}
void FPSCamera::moveY(GLfloat amount)
{
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeTranslate(tempMatrix2, 0.0f, amount, 0.0f);
	MathHelper::matrixMult4x4(this->mV, tempMatrix, tempMatrix2);
}
void FPSCamera::moveZ(GLfloat amount)
{
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeTranslate(tempMatrix2, 0.0f, 0.0f, amount);
	MathHelper::matrixMult4x4(this->mV, tempMatrix, tempMatrix2);
}
#pragma endregion TRANSFORM_METHODS

void FPSCamera::setPerspective(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane)
{
	GLfloat f = 1.0f/tan(fov*3.1415926f/360.0f);
	for (int i = 0; i < 16; i++) {
		mP[i] = 0.0f;
	}
	mP[0] = f/aspect;
	mP[5] = f;
	mP[10] = ((farPlane+nearPlane)/(nearPlane-farPlane));
	mP[11] = -1;
	mP[14] = (2.0f*farPlane*nearPlane)/(nearPlane-farPlane);
	mP[15] = 0;
}
void FPSCamera::setVPInShader(GLuint shaderID)
{
	static int counter = 0;
	
	// Find the location of the "mP" variable in the GPU vertex shader
	GLuint mpLoc = glGetUniformLocation(shaderID, "mP");
	// Send our variable mP to the GPU shader's mP variable
	glUniformMatrix4fv(mpLoc, 1, GL_TRUE, mP);
	// Do the same thing: set the GPU's mV in the shader passing our local mV member variable
	GLuint mvLoc = glGetUniformLocation(shaderID, "mV");
	glUniformMatrix4fv(mvLoc, 1, GL_TRUE, mV);

	if (counter == 0) {
		counter++;
		printf ("Loc of mP is %d and Loc of mV is %d\n", mpLoc, mvLoc);
	}
}