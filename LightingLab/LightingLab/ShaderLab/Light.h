#ifndef LIGHT
#define LIGHT
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Shader.h"

class Light {
public:
	Light(GLfloat* position, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, Shader* shader);
	void setLightPositionInShader();
	void setLightPosition(GLfloat* newPosition);

	GLfloat*	position;
	GLfloat*	ambient;
	GLfloat*	diffuse;
	GLfloat*	specular;
	Shader*		shader;

private:
	GLuint		lightPositionID;		// The ID of the lightPosition variable in the shader
};
#endif