#ifndef MODEL
#define MODEL
#include "Shader.h"
#include "MathHelper.h"
#include "Light.h" 
#include "OBJParser.h"


class Model {
public:
	Model(Shader* shader);
	~Model();
	void setGeometry(GLfloat vertices[], int size);
	void setNormals(GLfloat* normals, int size);
	void setVertexColors(GLfloat* colors, int size);
	void setTextureCoordinates(GLfloat* uvs, int size);
	void setIndexBuffer(GLuint* indices, int size);	
	void setMaterial (GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess);
	void setLight (Light* light);
	
	void loadVBO();
	void render();

	GLfloat scale;

	
	GLfloat*	mR;			// A rotation matrix (used in the shader to rotate normals)

	GLfloat*	mM;			// A matrix holding all orientation information for this model.
							// This will get passed off the mV variable in the shader
							// This is the "M" part of the MVP matrix
	Shader*		shader;		// The shader object for this model
	Light*		light;		// The *only* light for this object

	GLfloat xOffset, yOffset, zOffset;
private:
	
	GLfloat*	vertices;	// x, y, z
	GLfloat*	normals;	// Vertex normals
	GLfloat*	uvs;		// Texture coordinates
	GLfloat*	colors;		// Colors for each vertex
	GLuint*		indices;	// Index buffer							
	GLuint		numVertices, numNormals, numUVs, numColors, numIndices;
	GLuint		vao;		// Vertex Array Object
	GLuint		vbo;		// Vertex Buffer Object
	GLuint		index_vbo;	// Index Buffer Object	
	GLfloat*	ambient, *combinedAmbient;	// Ambient color				NEW!!
	GLfloat*	diffuse, *combinedDiffuse;	// Diffuse color				NEW!!
	GLfloat*	specular, *combinedSpecular;	// Specular color				NEW!!
	GLfloat		shininess;	// Shininess					NEW!!

	GLint		vPosition;	// A reference to the variable "vPosition" in the shader
	GLint		vNormal;	// A refernece to the variable "vNormal" in the shader
	GLint		vTexCoord;	// A reference to the variable "vTexCoord" in the shader
	GLint		vColor;		// A reference to the variable "vColor" in the shader
	GLint		umM;		// A reference to the variable "mM" in the shader
	GLint		umR;		// A reference to the variable "mR" in the shader
	GLint		ambientID;	// A reference to the ambient variable		NEW!!
	GLint		diffuseID;	// A reference to the diffuse variable		NEW!!
	GLint		specularID;	// A reference to the specular variable		NEW!!
	GLint		shineID;	// A reference to the shininess variable	NEW!!

	

};
#endif