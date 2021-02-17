#include <iostream>
using namespace std;

//--- OpenGL ---
#include <GL\glew.h>
#include <GL\wglew.h>
#pragma comment(lib, "glew32.lib")
//--------------

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GL\freeglut.h>
#include "Shader.h"
#include "Square.h"

#include <iostream>
using namespace std;


glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
int screenWidth = 480, screenHeight = 480;

Shader myShader;
Square myGreenSquare;
Square myBlackSquare;

float Angle = 0.0f;

//OPENGL FUNCTION PROTOTYPES
void display();				//used as callback in glut for display.
void reshape(int width, int height); //used as callback for reshape function in glut
void init();				//called in main when the program starts.

/*************    START OF OPENGL FUNCTIONS   ****************/

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width;
	screenHeight = height;

	//set Viewport dimensions
	glViewport(0, 0, width, height);						

	float ratio = (float)screenHeight / (float) screenWidth;


	//set the projection matrix
	ProjectionMatrix = glm::ortho<float>(-15, 15, -15*ratio , 15*ratio);
}


void display()									
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the view matrix - in this case just the identity matrix
	glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
	
	//Set up modelviewmatrix for the greensquare
	glm::mat4 ModelViewMatrix = glm::rotate(ViewMatrix, Angle, glm::vec3(0.0, 0.0, 1.0));
	myGreenSquare.Render(myShader, ModelViewMatrix, ProjectionMatrix);

	//Render the black square
	ModelViewMatrix = glm::rotate(ViewMatrix, Angle, glm::vec3(0.0, 0.0, 1.0));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(10, 0, 0));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, Angle, glm::vec3(0.0, 0.0, 1.0));
	myBlackSquare.Render(myShader, ModelViewMatrix, ProjectionMatrix);

	glutSwapBuffers();

	glutPostRedisplay();

	Angle += 0.05f;
	if (Angle >= 2.0*3.14159265) 
		Angle = 0;
}

void init()
{
	if (!myShader.load("Basic", "./shaders/basicTransformations.vert", "./shaders/basicTransformations.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}

	glClearColor(1.0,1.0,1.0,0.0);						//sets the clear colour to black
	
	myGreenSquare.SetSideSize(4.0f);
	float green[3] = { 0,1,0 };
	myGreenSquare.Init(myShader, green);

	myBlackSquare.SetSideSize(2.0f);
	float black[3] = { 0,0,0 };
	myBlackSquare.Init(myShader, black);
}

/**************** END OPENGL FUNCTIONS *************************/

// FREEGLUT WINDOW SET UP
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL FreeGLUT Example: Square");

	glutReshapeFunc(reshape);

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	//initialise the objects for rendering
	init();

	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}

