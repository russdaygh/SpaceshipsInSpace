// opening_a_window using glut example
//
//////////////////////////////////////////////////////////////////////////////////////////
// includes 
//////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include <time.h>
float elapsed = 0.0f;
float start_time = 0.0f;

#include "GL\glut.h"

#include "RustyLib.h"

using RustyLib::Vector3;
using RustyLib::Graphics::Renderer;

Renderer* render;

#include "Game.h"
Game* game;

//////////////////////////////////////////////////////////////////////////////////////////
// renderScene() - render the scene
//////////////////////////////////////////////////////////////////////////////////////////

void renderScene(void )
{
	// clear the back buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render code here...	
	game->Render(*render);

	// swap the buffers of the current window
	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////////////////////
// winReshapeFunc() - gets called initially and whenever the window get resized
// resizing has been locked
//////////////////////////////////////////////////////////////////////////////////////////
void winReshapeFunc(GLint w, GLint h)
{
	// specify current matrix
	glMatrixMode(GL_PROJECTION);

	// load an identity matrix
	glLoadIdentity();	

	// create a projection matrix... i.e. 2D projection onto xy plane
	glOrtho( 0, 1000, 1000, 0, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set up the viewport
	glViewport
	(
		0,				// lower left x position
		0,				// lower left y position
		(GLsizei) 500,	// viewport width
		(GLsizei) 500	// viewport height
	);
}

//////////////////////////////////////////////////////////////////////////////////////////
// update() - update function
//////////////////////////////////////////////////////////////////////////////////////////
void update(void )
{
	start_time = glutGet(GLUT_ELAPSED_TIME);
	elapsed =  start_time - elapsed;

	float timeStep = (float)elapsed / 100;

	game->Update(timeStep);

	RustyLib::Audio::Update(timeStep);

	renderScene();
}

//////////////////////////////////////////////////////////////////////////////////////////
// resolveKeyInput() - key input resolution function
//////////////////////////////////////////////////////////////////////////////////////////
void resolveKeyInput(unsigned char key, int y, int x)
{
	switch(key)
	{
	default:
		break;
	}

	game->ResolveKeyInput(key);
}

void resolveKeyUp(unsigned char key, int y, int x)
{
	game->ResolveKeyUp(key);
}

void mouseMove(int x, int y)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
// _tmain() - program entry point
//////////////////////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	// initialise the glut library
	glutInit(&argc, argv);

	// set up the initial display mode
	// need both double buffering and z-buffering
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	render = Renderer::Instance();

	Vector3 clipMin = Vector3(0,0,0);
	Vector3 clipMax = Vector3(1000,1000,0);
	render->SetClippingRegion(clipMin, clipMax);
	render->SetRenderMode(RustyLib::Graphics::RenderMode::Wireframe);
	render->SetClipMode(RustyLib::Graphics::ClipMode::CohenSutherland);

	start_time = glutGet(GLUT_ELAPSED_TIME);

	RustyLib::Audio::InitialiseRustyAL();

	game = new Game();
	game->Initialise();

	int res[2];
	game->GetWindowResolution(res);

	// set the initial window position
	glutInitWindowPosition(100, 100);

	// set the initial window size
	glutInitWindowSize(res[0], res[1]);

	// create and name the window
	glutCreateWindow("Games Content Submission by Russell Day");

	// reshape callback for current window
	glutReshapeFunc(winReshapeFunc);

	// set display callback for current window
	glutDisplayFunc(renderScene);

	// set up the global idle callback
	glutIdleFunc(update);

	//Set up the timed update
	//glutTimerFunc(16, renderScene, 0);

	//Assign keyboard down callback function
	glutKeyboardFunc(resolveKeyInput);

	//Assign keyboard up callback
	glutKeyboardUpFunc(resolveKeyUp);

	glutMotionFunc(mouseMove);

	// good place for one-off initialisations and objects creation..

	// enter the glut event processing loop
	glutMainLoop();

	return 0;
}