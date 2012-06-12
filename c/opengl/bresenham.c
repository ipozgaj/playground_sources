/* bresenham.c - Crtanja linija u OpenGL-u pomocu ugradjene funkcije i Bresenhamovog postupka
 * (C) 2006 Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
*/

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
#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480
#define WINDOW_POS_X	((SCREEN_WIDTH - WINDOW_WIDTH) / 2)
#define WINDOW_POS_Y	((SCREEN_HEIGHT - WINDOW_HEIGHT) / 2)

#define DIRECTION_RIGHT	1
#define DIRECTION_LEFT	-1
#define DIRECTION_UP	1
#define DIRECTION_DOWN	-1

struct point {
	double x, y;
} points[2];

int fullscreen = 0, p = 0;
int window_width = WINDOW_WIDTH, window_height = WINDOW_HEIGHT;

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

void draw_line(int ax, int ay, int bx, int by)
{
	float d;
	int i, x, y, dx, dy, direction_x, direction_y;
	
	glBegin(GL_LINES);
	glVertex2i(ax, ay + 5);
	glVertex2i(bx, by + 5);
	glEnd();

	x = ax;
	y = ay;
	dx = bx - ax;
	dy = by - ay;

	/* odredi smjer prirasta pravca */
	if (dx > 0 && dy < 0) {
		direction_x = DIRECTION_RIGHT;
		direction_y = DIRECTION_DOWN;
	} else if (dx < 0 && dy < 0) {
		direction_x = DIRECTION_LEFT;
		direction_y = DIRECTION_DOWN;
	} else if (dx < 0 && dy > 0) {
		direction_x = DIRECTION_LEFT;
		direction_y = DIRECTION_UP;
	} else {
		direction_x = DIRECTION_UP;
		direction_y = DIRECTION_RIGHT;
	}

	/* apsolutni iznos prirasta */
	dx = abs(dx);
	dy = abs(dy);

	glBegin(GL_POINTS);
	
	if (abs(dx) > abs(dy)) {
		/* veci je prirast po x varijabli */
		d = (double) dy / dx - 0.5;
		for (i = 0 ; i <= dx ; ++i) {
			glVertex2i(x, y);
			if (d > 0) {
				y += direction_y;
				d -= 1;
			}
			x += direction_x;
			d += (double) dy / dx;
		}
	} else {
		/* veci je prirast po y varijabli */
		d = (double) dx / dy - 0.5;
		for (i = 0 ; i <= dy ; ++i) {
			glVertex2i(x, y);
			if (d > 0) {
				x += direction_x;
				d -= 1;
			}
			y += direction_y;
			d += (double) dx / dy;
		}
	}
	
	glEnd();
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

	p = 0;
		
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1);
	glColor3f(0, 1, 0);
}

void keyboard_handler(unsigned char key, int x, int y)
{
	switch (key) {
		case 'r':
			glColor3f(1, 0, 0);
			break;
			
		case 'g':
			glColor3f(0, 1, 0);
			break;
			
		case 'b':
			glColor3f(0, 0, 1);
			break;

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

	glRecti(window_width - 15, window_height - 15, window_width, window_height);
	glFlush();
}

void mouse_handler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		points[p].x = x;
		points[p].y = window_height - y;
		p ^= 1;

		if(!p)
			draw_line(points[0].x, points[0].y, points[1].x, points[1].y);
		else
			glVertex2i(x, window_height - y);
		
		printf("Kordinate tocke %d: (%d, %d)\n", 2 - p, x, y);
		glFlush();
	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		resize_scene(window_width, window_height);
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

	printf("Crtanje linija Bresenhamerovim postupkom\n");
	printf("Tipke r, g, b mijenjaju boju, q izlazi iz programa\n");
	
	glutMainLoop();
	
	return EXIT_SUCCESS;
}
