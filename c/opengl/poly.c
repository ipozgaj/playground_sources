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

#define MAX_POINTS	100

struct point {
	int x, y;
} points[MAX_POINTS];

struct coef {
	int a, b, c;
} coefs[MAX_POINTS];

int ymin, ymax;
int fullscreen = 0, current_point = 0, num_points;
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

void draw_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
}

void resize_scene(int width, int height)
{
	window_width = width;
	window_height = height;
	
	current_point = 0;
		
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1);
	glColor3f(0, 1, 0);
}

void color_poly()
{
	int i, x, y, L, D;
	int xmin, xmax, ymin, ymax;

	xmin = xmax = points[0].x;
	ymin = ymax = points[0].y;

	for (i = 0 ; i < num_points ; ++i) {
		if (points[i].x < xmin) xmin = points[i].x;
		if (points[i].x > xmax) xmax = points[i].x;
		if (points[i].y < ymin) ymin = points[i].y;
		if (points[i].y > ymax) ymax = points[i].y;
	}

	for (y = ymin ; y < ymax ; y++) {
		L = xmin;
		D = xmax;

		for (i = 0 ; i < num_points ; ++i) {
			if (coefs[i].a != 0) {
				x = (-coefs[i].b * y - coefs[i].c) / coefs[i].a;
			}

			if (points[i].y < points[i + 1].y && x > L) L = x;
			if (points[i].y > points[i + 1].y && x < D) D = x;
		}

		if (L < D) {
			glBegin(GL_LINES);
			glVertex2i(L, y);
			glVertex2i(D, y);
			glEnd();
		}
	}
}

void keyboard_handler(unsigned char key, int x, int y)
{
	switch (key) {
		case 'c':
			color_poly();
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

	glFlush();
}

void draw_poly()
{
	int i;

	glBegin(GL_LINES);
	
	for (i = 0 ; i < num_points ; ++i) {
		/* crtanje linija */
		glVertex2i(points[i].x, points[i].y);
		glVertex2i(points[i + 1].x, points[i + 1].y);

		/* racunanje koeficijenata smjera bridova */
		coefs[i].a = points[i].y - points[i + 1].y;
		coefs[i].b = -points[i].x + points[i + 1].x;
		coefs[i].c = points[i].x * points[i + 1].y - points[i + 1].x * points[i].y;
	}

	glEnd();
	glFlush();
}

int is_in_poly(int x, int y)
{
	int i;

	for (i = 0 ; i < num_points ; ++i)
		if (x * coefs[i].a + y * coefs[i].b + coefs[i].c > 0)
			return 0;
		
	return 1;
}

void mouse_handler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && current_point < num_points) {
		points[current_point].x = x;
		points[current_point].y = window_height - y;

		if (current_point == 0)
			points[num_points] = points[0];
		
		glBegin(GL_POINTS);
		glVertex2i(points[current_point].x, points[current_point].y);
		glEnd();
		glFlush();
		
		++current_point;
		
		printf("Kordinate tocke %d: (%d, %d)\n", current_point, x, y);
	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		resize_scene(window_width, window_height);
	}

	if (current_point == num_points && state == GLUT_DOWN) {
		draw_poly();
		
		if (is_in_poly(x, window_height - y))
			printf("Toèka (%d, %d) je unutar poligona\n", x, y);
		else
			printf("Toèka (%d, %d) nije unutar poligona\n", x, y);

		return;
	}

}

int main(int argc, char **argv)
{
	printf("Unesi broj toèaka: ");
	scanf("%d", &num_points);

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

	glutMainLoop();
	
	return EXIT_SUCCESS;
}
