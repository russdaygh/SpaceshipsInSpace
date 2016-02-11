// opening_a_window using glut example
//
//////////////////////////////////////////////////////////////////////////////////////////
// includes 
//////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "RustyLib.h"

using RustyLib::Framework::StopWatch;

StopWatch* deltaTimer;
double last_time;
int width = 800;
int height  = 600;

#include "Game.h"
Game game;

//////////////////////////////////////////////////////////////////////////////////////////
// renderScene() - render the scene
//////////////////////////////////////////////////////////////////////////////////////////
void renderScene()
{
	// clear the back buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render code here...
	

	game.Render();
	

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
	gluOrtho2D( 0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set up the viewport
	glViewport
	(
		0,				// lower left x position
		0,				// lower left y position
		(GLsizei) width,	// viewport width
		(GLsizei) height	// viewport height
	);
}

//////////////////////////////////////////////////////////////////////////////////////////
// update() - update function
//////////////////////////////////////////////////////////////////////////////////////////
void update()
{
	// add any update code here...

	//Get the delta time

	double currentTime = deltaTimer->GetTime();
	const double DELTA_TIME = currentTime - last_time;
	last_time = currentTime;

	game.Update(DELTA_TIME);

	// always re-render the scene..
	renderScene();
}

//////////////////////////////////////////////////////////////////////////////////////////
// kbUpFunc() - program exit point
//////////////////////////////////////////////////////////////////////////////////////////
void kbUpFunc(unsigned char key, int x, int y)
{
	RustyLib::Input::Keyboard::HandleKeyUp(key);
}

//////////////////////////////////////////////////////////////////////////////////////////
// kbDnFunc() - program exit point
//////////////////////////////////////////////////////////////////////////////////////////
void kbDnFunc(unsigned char key, int x, int y)
{
	RustyLib::Input::Keyboard::HandleKeyDown(key);
}

//////////////////////////////////////////////////////////////////////////////////////////
// exitFunc() - program exit point
//////////////////////////////////////////////////////////////////////////////////////////
void exitFunc(void )
{
	game.Exit();
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// set the initial window position
	glutInitWindowPosition(100, 100);

	// set the initial window size
	glutInitWindowSize(width, height);

	// create and name the window
	glutCreateWindow("Rusty Asteroids");

	// reshape callback for current window
	glutReshapeFunc(winReshapeFunc);

	// set display callback for current window
	glutDisplayFunc(renderScene);	

	// set up the global idle callback
	glutIdleFunc(update);

	//Set up key func
	glutKeyboardUpFunc(kbUpFunc);
	glutKeyboardFunc(kbDnFunc);

	//Set up exit function
	atexit(exitFunc);

	// good place for one-off initialisations and objects creation..

	RustyLib::Graphics::Renderer::Instance().SetClipMode(
		RustyLib::Graphics::ClipMode::CohenSutherland);
	RustyLib::Graphics::Renderer::Instance().SetClippingRegion(
		0, 0, 800, 600);
	RustyLib::Graphics::Renderer::Instance().SetLineAlgorithm(
		RustyLib::Graphics::LineAlgorithm::Bresenhams);
	RustyLib::Graphics::Renderer::Instance().SetTriAlgorithm(
		RustyLib::Graphics::TriAlgorithm::Take2);
	RustyLib::Graphics::Renderer::Instance().SetRenderMode(
		RustyLib::Graphics::RenderMode::SolidColour);
	
	deltaTimer = new StopWatch();

	game.Initialise();

	// enter the glut event processing loop
	glutMainLoop();	

	return 0;
}




