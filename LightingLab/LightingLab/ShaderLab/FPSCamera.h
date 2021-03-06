#ifndef CAMERA
#define CAMERA
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "MathHelper.h"

class FPSCamera {
public:
	FPSCamera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane);
	~FPSCamera();		// Destructor

	// Transform methods.  All take in an amount of rotation or translation and can be positive or negative.
	void rotateX(GLfloat amount);	// Look left/right
	void rotateY(GLfloat amount);	// Look up/down
	void rotateZ(GLfloat amount);	// Bank left/right (like an airplane).  This isn't commonly used in FPS.
	void moveX(GLfloat amount);		// Strafing left/right
	void moveY(GLfloat amount);		// Floating up/down or jumping
	void moveZ(GLfloat amount);		// Forward/backward

	void setVPInShader(GLuint shaderID); // This method sets the View and Perspective in the shader
	void setPerspective(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane);

private:
	GLfloat* mP;	// the "P"erspective matrix.  This is usually set once.
	GLfloat* mV;	// the "V"iew matrix.  This usually changes every frame.
	GLfloat* tempMatrix;	// I create this to prevent constantly allocating/deallocating memory
	GLfloat* tempMatrix2;	// Same here
};

#endif