/* Jednostavni OpenGL demo
 * (C) 2003  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdlib.h>
#include <math.h>

#ifndef __linux__
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define WINDOW_WIDTH  500.0f
#define WINDOW_HEIGHT 500.0f
#define X_START_POSITION ((SCREEN_WIDTH - WINDOW_WIDTH) / 2)
#define Y_START_POSITION ((SCREEN_HEIGHT - WINDOW_HEIGHT) / 2)

float xdelta = 0.01;
float ydelta = 0.05;

void init_gl ()
{
	glShadeModel (GL_SMOOTH);
	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth (1.0f);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);
	glEnable (GL_COLOR_MATERIAL);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void draw_scene ()
{	
	static float theta = 0;
	float x, y, t;
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	
	glTranslatef (0.0f, 0.0f, -5.0f);
	glRotatef (theta, 1.0f, 0.5f, 0.25f);
	
	glColor3f (0.0f, 1.0f, 0.0f);

	glBegin(GL_QUADS);
	glColor3f (0.0f, 1.0f, 0.0f);
	glVertex3i(-1, 1, 0);
	glColor3f (1.0f, 0.0f, 0.0f);
	glVertex3i(1, 1, 0);
	glColor3f (0.0f, 0.0f, 1.0f);
	glVertex3i(1, -1, 0);
	glColor3f (1.0f, 0.0f, 0.0f);
	glVertex3i(-1, -1, 0);
	glEnd();
	
	glBegin(GL_QUADS);
	glColor3f (0.0f, 1.0f, 0.0f);
	glVertex3i(-1, 1, 1);
	glColor3f (1.0f, 0.0f, 0.0f);
	glVertex3i(1, 1, 1);
	glColor3f (0.0f, 0.0f, 1.0f);
	glVertex3i(1, -1, 1);
	glColor3f (1.0f, 0.0f, 0.0f);
	glVertex3i(-1, -1, 1);
	glEnd();

	theta += 0.45;
	
	glutSwapBuffers ();
}

void resize_scene (int width, int height)
{
	glViewport (0, 0, width, height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	if (height == 0) ++height;

	gluPerspective (80, (float) width / height, 1.0f, 5000.0f);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void keyboard_handler (unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
		case 'q':
		case 'Q':
			exit (0);
	}
}

void special_keys_handler (int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_UP:
			glutFullScreen ();
			break;
		case GLUT_KEY_DOWN:
			glutReshapeWindow (WINDOW_WIDTH, WINDOW_HEIGHT);
			break;
	}
}

int main (int argc, char **argv)
{
	glutInit (&argc, argv);
	init_gl ();
	
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition (X_START_POSITION, Y_START_POSITION);
	glutCreateWindow (argv[0]);

	glutDisplayFunc (draw_scene);
	glutIdleFunc (draw_scene);
	glutReshapeFunc (resize_scene);
	glutKeyboardFunc (keyboard_handler);
	glutSpecialFunc (special_keys_handler);

	glutMainLoop ();
	
	return 0;
}

