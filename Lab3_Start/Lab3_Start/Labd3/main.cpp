#include <GL/GLEW.h>
#include <GL/freeglut.h>
#include <stdio.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define VERTICES 6
#define INDEX_NUM 9

void render() {
	//Put your drawing code here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
	glutCreateWindow("Lab 3");          // Create the window and give it a title

	glewInit();                         // Ask the driver for all the OpenGL functions

	// Some callback functions
	glutDisplayFunc(render);            // Use the render function to draw
	glutSpecialFunc(specialKeys);       // Use the specialKeys function for Up/Down/Left/Right keys
	glutMouseFunc(mousePressed);        // Use for mouse clicks
	glutMotionFunc(mouseDragged);       // Use for mouse dragging
	glutPassiveMotionFunc(mouseMoved); // Use for mouse moving

#pragma region SHADER_STUFF
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

	// Index Buffer
	GLuint indexBuffID;
	glGenBuffers(1, &indexBuffID);
	glBindVertexArray(

	// Make the program and link it all together
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

	// 1) Put your vertex data (arrays) here
	GLfloat vertices[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f };
	GLfloat colors[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 1.0f};
	// 2) Create/bind a Vertex Array Object (VAO)
	GLuint vertArrObj;
	glGenVertexArrays(1, &vertArrObj);
	glBindVertexArray(vertArrObj);
	// 3) Create/bind a Vertex Buffer Object (VBO)
	GLuint vertBuffer;
	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	// 4) Specify the size of the VBO.
	glBufferData(GL_ARRAY_BUFFER, 21*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// 5) Load the position info. 
	glBufferSubData( GL_ARRAY_BUFFER, 0, 9*sizeof(GLfloat), vertices);
	// 6) Load the color info.
	glBufferSubData( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), 12*sizeof(GLfloat), colors);
	// 7) Get the IDs of vPosition and vColor
	GLint vertexID;
	vertexID = glGetAttribLocation(shaderProgram, "vPosition");
	GLint colorID;
	colorID = glGetAttribLocation(shaderProgram, "vColor");
	// 8) Turn them on
	glEnableVertexAttribArray(vertexID);
	glEnableVertexAttribArray(colorID);
	// It's a good idea to print out those IDs.  If you see -1s, you did something wrong
	printf("vPosition ID is %d\n", vertexID);
	printf("vColor ID is %d\n", colorID);
	// 9) Tell vPosition where it can find its data
	glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// 10) Tell vColor where it can find its data
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(9*sizeof(GLfloat)));

	glutMainLoop();                     // Start listening for events
}