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
} coefs[MAX_POINTS], ray;

int ymin, ymax;
int fullscreen = 0, current_point = 0, num_points;
int window_width = WINDOW_WIDTH, window_height = WINDOW_HEIGHT;

int s[100];

void color_poly();

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

double set_t(int x1, int y1, int x2, int y2, int x3, int y3)
{
	int d1 = x2 - x1;
	int d2 = y2 - y1;
	double t;

	if (d1)
		t = (x3 - x1) / (double) d1;
	
	if (d2)
		t = (y3 - y1) / (double) d2;

	return t;
}

int crossings(int x0, int y0, struct coef ray)
{
	int i, x3, y3, w3, n0;
	int d1, d2;
	double t, t1;
	
	n0 = 0;
	
	for (i = 0 ; i < num_points ; ++i) {
		x3 = coefs[i].b * ray.c - coefs[i].c * ray.b;
		y3 = -coefs[i].a * ray.c + coefs[i].c * ray.a;
		w3 = coefs[i].a * ray.b - coefs[i].b * ray.a;

		if (w3 == 0)
			continue;

		x3 /= w3;
		y3 /= w3;

		t = set_t(x0, y0, x0 + 1, y0, x3, y3);

		if (t < 0)
			continue;
		
		t1 = t;
		t = set_t(points[i].x, points[i].y, points[i+1].x, points[i+1].y, x3, y3);
			
		if (t <= 0 || t > 1)
			continue;

		if (t == 1) {
			d1 = ray.a * points[i].x + ray.b * points[i].y + ray.c;
			d2 = ray.a * points[i+2].x + ray.b * points[i+2].y + ray.c;
		
			if ((((d1 > 0) && (d2 > 0)) || ((d1 < 0) && (d2 < 0))))
				continue;
		}

		s[n0++] = t1;
	}

	return n0;
}

int is_in_poly(int x, int y)
{
	struct coef ray = {0, 1, -y};

	if (crossings(x, y, ray) % 2)
		return 1;

	return 0;
}

void color_poly()
{
	int i, y, z, n, tmp;
	struct coef ray = {0, 1, 0};
	int ymin, ymax;

	ymin = ymax = points[0].y;

	for (i = 0 ; i < num_points ; ++i) {
		if (points[i].y < ymin) ymin = points[i].y;
		if (points[i].y > ymax) ymax = points[i].y;
	}

	for (y = ymin ; y < ymax ; y++) {
		ray.c = -y;
		n = crossings(0, y, ray);

		if (n < 2)
			continue;
		
		do {
			z = 0;
			for (i = 0 ; i < n - 1 ; ++i)
				if (s[i] > s[i+1]) {
					tmp = s[i];
					s[i] = s[i+1];
					s[i+1] = tmp;
					z = 1;
				}
		} while (z);
			
		for (i = 0 ; i < n - 1 ; i += 2) {
			glBegin(GL_LINES);
			glVertex2i(s[i], y);
			glVertex2i(s[i+1], y);
			glEnd();
		}
	}

	glFlush();
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
