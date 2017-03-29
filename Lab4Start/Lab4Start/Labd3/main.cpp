#include <GL/GLEW.h>
#include <GL/freeglut.h>
#include <stdio.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ********** Hmmm - this call doesn't look too good anymore!
	//glDrawArrays(GL_TRIANGLES, 0, 9);
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();
	glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: printf("The UP key was pressed\n"); break;
	case GLUT_KEY_DOWN: printf("The DOWN key was pressed\n"); break;
	}
}

void mousePressed(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		//printf("Mouse clicked at %d %d\n", x, y);
	}
}

void mouseMoved(int x, int y) {
	//printf("Mouse moved at %d %d\n", x, y);
}

void mouseDragged(int x, int y) {
	//printf("Mouse dragged at %d %d\n", x, y);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);              // Initialize GLUT
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Set up buffers
	glutInitWindowPosition(200, 200);   // Optional: position the upper-left of the window
	glutInitWindowSize(800, 600);       // Set the window size
	glutCreateWindow("Lab 4");          // Create the window and give it a title

	glewInit();                         // Ask the driver for all the OpenGL functions

	// Some callback functions
	glutDisplayFunc(render);            // Use the render function to draw
	glutSpecialFunc(specialKeys);       // Use the specialKeys function for Up/Down/Left/Right keys
	glutMouseFunc(mousePressed);        // Use for mouse clicks
	glutMotionFunc(mouseDragged);       // Use for mouse dragging
	glutPassiveMotionFunc(mouseMoved); // Use for mouse moving

#pragma region SHADER_STUFF
	// =========  SHADER STUFF  ===============
	const GLchar* vertexShaderCode = "#version 150\n\
									in vec4 vPosition;\n\
									in vec4 vColor;\n\
									out vec4 color;\n\
									void main () {\n\
										color = vColor;\n\
										\tgl_Position = vPosition;\n\
									 }\n";

	const GLchar* fragmentShaderCode = "#version 150\n\n\
										out vec4 fColor;\n\
										in vec4 color;\n\
										void main () {\
										   fColor = color;\n\
							  			}";
	// Vertex Shader 
	GLint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShaderID);
	GLint compiled = -10;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compiled);
	printf("Vertex compile status %d!\n", compiled);
	printf("Vertex shader ID is %d\n", vertexShaderID);

	// Fragment Shader 
	GLint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShaderID);
	GLint compiled2 = -19;
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compiled2);

	printf("Fragment compile status %d!\n", compiled2);
	printf("Fragment shader ID is %d\n", fragmentShaderID);

	// Make the program
	GLint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);
	glLinkProgram(shaderProgram);
	GLint linkedStatus = 14;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkedStatus);
	printf("Link status is %d\n", linkedStatus);
	printf("Shader program ID is %d\n", shaderProgram);

	glUseProgram(shaderProgram);
#pragma endregion SHADER_STUFF


	// **********  THIS IS TOO MANY VERTICES!  FIX IT!
	// Note: I've helped out by commenting the vertices according to the
	// picture in the lab.  You'll want to put the vertices in order, such
	// that 0 is first, 1 is second, 2 is third...
	//GLfloat vertices[] = { -0.5f, -0.5f, 0.0f,  // 0
	//	0.0f, -0.5f, 0.0f, // 3
	//	-0.25f, 0.0f, 0.0f, // 1
	//	-0.25f, 0.0f, 0.0f, // 1
	//	0.25f, 0.0f, 0.0f, // 4
	//	0.0f, 0.5f, 0.0f, // 2
	//	0.0f, -0.5f, 0.0f, // 3
	//	0.5f, -0.5f, 0.0f, // 5
	//	0.25f, 0.0f, 0.0f // 4
	//};
	GLfloat vertices[] = { 0.0f, 0.5f, 0.0f, //first vertex
		-.25f, 0.0f, 0.0f, //second
		0.25f, 0.0f, 0.0f, //third
		-0.5f, -0.5f, 0.0f, //fourth
		0.0f, -0.5f, 0.0f, //fifth
		0.5f, -0.5f, 0.0f //Sixth
		};
	

	// ********** SIMILARLY, THIS IS TOO MANY COLORS!
	//GLfloat colors[] = { 1.0f, 0.73f, 0.0f, 1.0f, //0
	//	1.0f, 0.65f, 0.0f, 1.0f, // 3
	//	1.0f, 1.0f, 0.0f, 1.0f, // 1
	//	1.0f, 0.73f, 0.0f, 1.0f, // 1
	//	1.0f, 0.65f, 0.0f, 1.0f, // 4
	//	1.0f, 1.0f, 0.0f, 1.0f, // 2
	//	1.0f, 0.73f, 0.0f, 1.0f, // 3
	//	1.0f, 0.65f, 0.0f, 1.0f, // 5
	//	1.0f, 1.0f, 0.0f, 1.0f, // 4
	//};
	GLfloat colors[] = { 1.0f, 1.0f, 0.0f, 1.0f, //first color
		1.0f, 1.0f, 0.0f, 1.0f, //second color
		1.0f, 0.65f, 0.0f, 1.0f, //third color
		1.0f, 0.73f, 0.0f, 1.0f, //fourth color
		1.0f, 0.73f, 0.0f, 1.0f, //fifth color
		1.0f, 0.65f, 0.0f, 1.0f //sixth color
		};
	
	// ********* CREATE AN INDEX ARRAY RIGHT HERE.  AGAIN, LOOK AT THE LAB.
	GLuint indexArray[] = { 0, 1, 2, 1, 3, 4, 2, 4, 5};

	// ********  HMMMM - something weird here
	int numVertices = 6;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);	// Use the Vertex Array Object we created above

	GLuint vbo;		// The Vertex Buffer Object ID
	glGenBuffers(1, &vbo);	// Ask the GPU driver for a buffer array. "vbo" now has the ID
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	// Make this buffer the active one for subsequent operations (below)

	// Specify how big the buffer is, but don't pass the data yet (see NULL).  We *could* have, but I wanted to show glBufferSubData
	glBufferData(GL_ARRAY_BUFFER, numVertices * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// NOW we copy the data in as a separate step. There is an "offset" of 0 - meaning the beginning of the buffer. 
	// We specify the size of the data we want to copy, followed by a pointer to the actual data
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), numVertices * 4 * sizeof(GLfloat), colors);

	// Figure out where vPosition is in our shader and get its ID
	GLuint loc = glGetAttribLocation(shaderProgram, "vPosition");
	GLuint loc2 = glGetAttribLocation(shaderProgram, "vColor");
	glEnableVertexAttribArray(loc);
	glEnableVertexAttribArray(loc2);

	printf("vPosition ID is %d\n", loc);
	printf("vColor ID is %d\n", loc2);

	// When it's time for vPosition to find information, we need to tell it where the data is (or how the data should be parsed)
	// Here, we're saying that "vPosition" (loc from above) should look for 3 GLfloats. The data isn't normalized or interlaced, 
	// and starts at index 0 (the beginning of the current buffer)
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(loc2, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices * 3 * sizeof(GLfloat)));
	
	// ********  NOW, CREATE AN INDEX BUFFER!
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 9*sizeof(GLuint), indexArray, GL_STATIC_DRAW);
	

	glutMainLoop();                     // Start listening for events
}