#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef __linux__
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	768
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define WINDOW_POS_X	((SCREEN_WIDTH - WINDOW_WIDTH) / 2)
#define WINDOW_POS_Y	((SCREEN_HEIGHT - WINDOW_HEIGHT) / 2)

int fullscreen = 0;
int window_width = WINDOW_WIDTH, window_height = WINDOW_HEIGHT;

int xmax = WINDOW_WIDTH, ymax = WINDOW_HEIGHT, m;
float umin, umax, vmin, vmax, epsilon;
float oldumin, oldumax, oldvmin, oldvmax;

int point = 0, xcords[2], ycords[2];

void init_gl()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0.5);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void draw_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
}

void resize_scene(int width, int height)
{
	window_width = width;
	window_height = height;

	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1);
	glColor3f(0, 1, 0);
}

void crtaj_fraktal()
{
	int x0, y0, k;
	float u0, v0, r, zreal, zimag, creal, cimag, tmpre, tmpim, xratio, yratio;

	yratio = (vmax - vmin) / ymax;
	xratio = (umax - umin) / xmax;
	
	glBegin(GL_POINTS);

	for (y0 = 0 ; y0 < ymax ; ++y0) {
		v0 = yratio * y0 + vmin;

		for (x0 = 0 ; x0 < xmax ; ++x0) {
			u0 = xratio * x0 + umin;
			
			k = -1;
			creal = u0;
			cimag = v0;
			zreal = zimag = 0;

			do {
				++k;
				tmpre = zreal * zreal - zimag * zimag;
				tmpim = 2 * zreal * zimag;
				zreal = tmpre + creal;
				zimag = tmpim + cimag;
				r = sqrt(zreal * zreal + zimag * zimag);
			} while ((r < epsilon) && (k < m));

			glColor3f(0, 0, 1.0 / m * k);
			glVertex2i(x0, window_height - y0);
		}
	}

	glEnd();
	glFlush();
		
}

void keyboard_handler(unsigned char key, int x, int y)
{
	switch (key) {
		case 'f':
			if (!fullscreen)
				glutFullScreen();
			else
				glutReshapeWindow(window_width, window_height);
			
			fullscreen ^= 1;
			break;

		case 'q':
			exit(EXIT_SUCCESS);
	}

	glFlush();
}

void mouse_handler(int button, int state, int x, int y)
{
	float tmpx, tmpy, shiftx, shifty;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		xcords[point] = x;
		ycords[point] = y;
		++point;

		if (point == 2) {
			glColor3f(1, 1, 1);
			glBegin(GL_LINES);
			glVertex2i(xcords[0], WINDOW_HEIGHT - ycords[0]);
			glVertex2i(xcords[1], WINDOW_HEIGHT - ycords[0]);
			glVertex2i(xcords[1], WINDOW_HEIGHT - ycords[0]);
			glVertex2i(xcords[1], WINDOW_HEIGHT - ycords[1]);
			glVertex2i(xcords[1], WINDOW_HEIGHT - ycords[1]);
			glVertex2i(xcords[0], WINDOW_HEIGHT - ycords[1]);
			glVertex2i(xcords[0], WINDOW_HEIGHT - ycords[1]);
			glVertex2i(xcords[0], WINDOW_HEIGHT - ycords[0]);
			glEnd();
			glFlush();
			point = 0;

			tmpx = (umax - umin) / WINDOW_WIDTH;
			tmpy = (vmax - vmin) / WINDOW_HEIGHT;

			shiftx = umin;
			shifty = vmin;

			umin = xcords[0] * tmpx + shiftx;
			umax = xcords[1] * tmpx + shiftx;
			vmin = (WINDOW_HEIGHT - ycords[1]) * tmpy + shifty;
			vmax = (WINDOW_HEIGHT - ycords[0]) * tmpy + shifty;

			crtaj_fraktal();
		}

	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		umin = oldumin;
		umax = oldumax;
		vmin = oldvmin;
		vmax = oldvmax;
		
		crtaj_fraktal();
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	init_gl(); 
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutCreateWindow(argv[0]);
	
	glutDisplayFunc(draw_scene);
	glutReshapeFunc(resize_scene);
	glutKeyboardFunc(keyboard_handler);
	glutMouseFunc(mouse_handler);

	epsilon = 200;
	m = 100;
	umin = -2.2; umax = 0.6;
	vmin = -1.3; vmax = 1.3;	

	oldumin = umin;
	oldvmin = vmin;
	oldumax = umax;
	oldvmax = vmax;

	printf("Mandelbrotov skup\n=================\n");
	printf("Desni klik - pocetni skup\n");
	printf("Lijevi klik - odabir tocke (gornja lijeva, donja desna) pravokutnika za zoom\n");
	
	glutMainLoop();
	
	return EXIT_SUCCESS;
}
