#include "Model.h"

// From http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Model::Model(Shader* shader) {
	this->shader = shader;
	glGenVertexArrays(1, &vao);							// Generate a VAO
	glGenBuffers(1, &vbo);								// Generate a buffer object
	glGenBuffers(1, &index_vbo);
	vertices = normals = uvs = colors = NULL;			// Assume no vertex information
	numVertices = numNormals = numUVs = numColors = 0;	// Set all sizes to 0
	ambient = diffuse = specular = NULL;				// NEW!!
	shininess = 0.0f;									// NEW!!
	vPosition = vNormal = vTexCoord = vColor = umM = -1;
	ambientID = diffuseID = specularID = shineID = -1;	// NEW!!

	mM = new GLfloat[16];
	mR = new GLfloat[16];

	scale = 1.0f;

	MathHelper::makeIdentity(mM);						// By default, the Model matrix is an identity matrix.
														// Remember, an identity matrix has no effect

	umM = glGetUniformLocation(shader->programID, "mM");// Find the mM variable in the shader 
	umR = glGetUniformLocation(shader->programID, "mR");
	printf ("mM variable in shader is %d\n", umM);
}
Model::~Model() {
	glDisableVertexAttribArray(vPosition);				// Cleanup the mess we've made
	glDisableVertexAttribArray(vNormal);
	glDisableVertexAttribArray(vTexCoord);
	glDisableVertexAttribArray(vColor);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	delete (shader);
	delete (vertices);
	delete (normals);
	delete (indices);
	delete (uvs);
	delete (colors);
	delete (ambient);
	delete (diffuse);
	delete (specular);
}

#pragma region SET_DATA_FUNCTIONS
void Model::setGeometry(GLfloat* vertices, int size){
	printf ("Copying %d vertices\n", size/sizeof(GLfloat)/3);
	this->vertices = vertices;
	this->numVertices = size/sizeof(GLfloat)/3;
	vPosition = glGetAttribLocation(shader->programID, "vPosition");
	printf ("vPosition ID is: %d\n", vPosition);
}
void Model::setNormals(GLfloat* normals, int size){
	printf ("Copying %d normals....\n", size/sizeof(GLfloat)/3);
	this->normals = normals;
	this->numNormals = size/sizeof(GLfloat)/3;
	vNormal = glGetAttribLocation(shader->programID, "vNormal");
	printf ("vNormal ID is: %d\n", vNormal);
}
void Model::setVertexColors(GLfloat* colors, int size){
	printf ("Copying %d colors...\n", size/sizeof(GLfloat)/4);
	this->colors = colors;
	this->numColors = size/sizeof(GLfloat)/4;
	vColor = glGetAttribLocation(shader->programID, "vColor");
	printf ("vColor ID is: %d\n", vColor);
}
void Model::setTextureCoordinates(GLfloat* uvs, int size){
	printf ("Copying uvs...\n");
	this->uvs = uvs;
	this->numUVs = size/sizeof(GLfloat)/2;
	vTexCoord = glGetAttribLocation(shader->programID, "vTexCoord");
	printf ("vTexCoord ID is: %d\n", vTexCoord);
}

void Model::setIndexBuffer(GLuint* indices, int size){					
	printf ("Copying %d indices...\n", size);
	this->indices = indices;
	this->numIndices = size/sizeof(GLuint);
	glBindVertexArray(vao);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_vbo);
	// This time, we'll just go ahead and pass the data off to the buffer because
	// we're not packing multiple data sets into the buffer
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	printf ("Loaded %d indices\n", this->numIndices);
}
// Set the material of this model
void Model::setMaterial (GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess)
{
	if (this->ambient==NULL) { 
		this->ambient = new GLfloat[4];
		this->combinedAmbient = new GLfloat[4];	// Combination of material and light
		ambientID = glGetUniformLocation(shader->programID, "ambient");
		printf ("Ambient ID is %d\n", ambientID);
	}
	if (this->diffuse==NULL) {
		this->diffuse = new GLfloat[4];
		this->combinedDiffuse = new GLfloat[4];	// Combination of material and light
		diffuseID = glGetUniformLocation(shader->programID, "diffuse");
		printf ("Diffuse ID is %d\n", diffuseID);
	}
	if (this->specular==NULL) {
		this->specular = new GLfloat[4];
		this->combinedSpecular = new GLfloat[4];	// Combination of material and light
		specularID = glGetUniformLocation(shader->programID, "specular");
		printf ("Specular ID is %d\n", specularID);
	}

	for (int i = 0; i < 4; i++) {
		this->ambient[i] = ambient[i];
		this->diffuse[i] = diffuse[i];
		this->specular[i] = specular[i];
	}
	this->shininess = shininess;
	shineID = glGetUniformLocation(shader->programID, "shininess");
}

void Model::setLight(Light* light) {
	this->light = light;
}
#pragma endregion SET_DATA_FUNCTIONS



// Pre-condition: You must have called at least one of the methods above
// This method loads all of the data onto the GPU.
void Model::loadVBO() {
	// Bind the vao
	glBindVertexArray(vao);
	// Bind vbo as the current VBO. Note: the VBO was created in the constructor
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	int offset = 0;
	
	printf ("Loading data onto the GPU...\n");
	// Calculate the size of the buffer we need
	int sizeBuffer = (numVertices*3+numNormals*3+numUVs*2+numColors*4)*sizeof(GLfloat);
	
	// Tell OpenGL that this data probably won't change and is used for drawing (GL_STATIC_DRAW).
	// We don't pass any data yet (NULL), but we specify the size (sizeBuffer). We'll pass data 
	// later using glBufferSubData
	
	// Call glBufferData and tell the GPU how big the buffer is.  Do NOT load any data yet.
	glBufferData(GL_ARRAY_BUFFER, sizeBuffer, NULL, GL_STATIC_DRAW);

	// If the vertices aren't NULL, load them onto the GPU. Offset is currently 0.
	if (vertices) {
		printf ("Loading vertices onto GPU...\n");
		// Load the vertices into the GPU buffer. Note the size is the number of vertices * 3(x, y, z)
		glBufferSubData(GL_ARRAY_BUFFER, offset, numVertices*3*sizeof(GLfloat), this->vertices);
		// You should read this as "vPosition should read in 3 GL_FLOATs that aren't normalized (GL_FALSE).
		// The data has a stride of 0 (non-interlaced) and starts with an offset of 0 in the buffer."
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
		offset+=numVertices*3*sizeof(GLfloat);
	}
	// Load in the vertex normals right after the vertex coordinates.  Remember, there are 3 values for a normal
	if (normals) {
		printf ("Loading normals onto GPU...\n");
		
		// Load in the vertex data onto the GPU using glBufferSub. Put it right after the vertices
		glBufferSubData(GL_ARRAY_BUFFER, offset, numNormals*3*sizeof(GLfloat), this->normals);
		// Note that the normal data has an offset that starts after the vertex data in the buffer
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
		offset+=numNormals*3*sizeof(GLfloat);
		
	}
	// Load in the texture coordinates right after the normals. Remember, there are 2 values for each uvs
	if (uvs) {
		printf ("Loading UVs onto GPU...\n");
		
		// Call glBufferSubData to put the UVs onto the GPU. CAREFUL! There's only 2 UVs
		glBufferSubData(GL_ARRAY_BUFFER, offset, numUVs*2*sizeof(GLfloat), this->uvs);
		// Call glVertexAttribPointer to tell vTexCoord where to find it's data. 
		glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
		offset+=numUVs*2*sizeof(GLfloat);
	}
	// Load in the color coordinates right after the texture coordinates. There are 4 values for a color
	if (colors) {
		printf ("Loading colors onto GPU...\n");
		
		// Put the color info onto the GPU. CAREFUL! There are 4 parts to each color
		glBufferSubData(GL_ARRAY_BUFFER, offset, numColors*4*sizeof(GLfloat), this->colors);
		// ... and getll vColor where to find it's data there.
		glVertexAttribPointer (vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(offset));
		offset+=numColors*4*sizeof(GLfloat);
	}
}

void Model::render(){
	glBindVertexArray(vao);						// Activate the VAO again. All that buffer info is remembered
	glUseProgram(shader->programID);			// Use the shader program
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (vertices) {
		glEnableVertexAttribArray(vPosition);	// It's a pain, but we always have to enable shader variables
	}
	if (normals) {
		glEnableVertexAttribArray(vNormal);
	}
	if (vTexCoord != -1) {
		glEnableVertexAttribArray(vTexCoord);
	}
	if (vColor != -1) {
		glEnableVertexAttribArray(vColor);
	}
	if (umM != -1) {
		// Pass off our Model matrix (called "mM" from the MVP matrix)
		glUniformMatrix4fv(umM, 1, GL_TRUE, mM);
	}
	if (umR != -1) {
		glUniformMatrix4fv(umR, 1, GL_TRUE, mR);
	}
	if (ambient) {
		// We have light in this scene!
		// First, calculate the color using the material AND the light
		for (int i = 0; i < 4; i++) {
			combinedAmbient[i] = ambient[i]*light->ambient[i];
			combinedDiffuse[i] = diffuse[i]*light->diffuse[i];
			combinedSpecular[i] = specular[i]*light->specular[i];
		}
		// Pass that info off to the shader
		glUniform4fv(ambientID, 1, combinedAmbient);
		glUniform4fv(diffuseID, 1, combinedDiffuse);
		glUniform4fv(specularID, 1, combinedSpecular);
		glUniform1f(shineID, shininess);
	}
	// Now, we can draw two different ways - depending on whether or not the index buffer
	// is alive/active.  If indices is NULL, then we know to draw triangles.  If it's not,
	// then we draw elements
	if (!indices) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}else {
		// 1) What kind of elements we're drawing
		// 2) The number of indices we're drawing (can be less than numIndices)
		// 3) The data type of the indices
		// 4) An offset within the buffer of where we should start drawing
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	}

	if (vertices) {
		glDisableVertexAttribArray(vPosition);	// Disable the variables
	}
	if (normals) {
		glDisableVertexAttribArray(vNormal);
	}
	if (vTexCoord!=-1) {
		glDisableVertexAttribArray(vTexCoord);
	}
	if (vColor!=-1) {
		glDisableVertexAttribArray(vColor);
	}
}