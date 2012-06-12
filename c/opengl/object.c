#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define ROTATION 1

struct point {
	float x, y, z;
} points[5000];

struct poligon {
	int v1, v2, v3;
} poligons[10000];

struct coef {
	float a, b, c, d;
} coefs[10000];

int zoom = -5;
int num_points, num_poligons;
int fullscreen = 0, num_points;
int window_width = WINDOW_WIDTH, window_height = WINDOW_HEIGHT;
float theta = 0;

void init_gl()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0.5f);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void draw_scene()
{
	int i;
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef (0.0, 0.0, zoom);
    glRotatef (theta, 1.0, 0.5, 0.25);
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);

	for (i = 1 ; i < num_poligons ; ++i) {
		glVertex3f(points[poligons[i].v1].x, points[poligons[i].v1].y, points[poligons[i].v1].z);
		glVertex3f(points[poligons[i].v2].x, points[poligons[i].v2].y, points[poligons[i].v2].z);
		glVertex3f(points[poligons[i].v3].x, points[poligons[i].v3].y, points[poligons[i].v3].z);
		glVertex3f(points[poligons[i].v1].x, points[poligons[i].v1].y, points[poligons[i].v1].z);
		glVertex3f(points[poligons[i].v2].x, points[poligons[i].v2].y, points[poligons[i].v2].z);
		glVertex3f(points[poligons[i].v3].x, points[poligons[i].v3].y, points[poligons[i].v3].z);
	}
	
	glEnd();
	theta += ROTATION; 
	glutSwapBuffers();
}

void resize_scene(int width, int height)
{
	window_width = width;
	window_height = height;
	
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (80, (float) window_width / window_height, 1.0, 6000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

		case 'y':
			zoom++;
			break;

		case 'x':
			zoom--;
			break;
			
		case 'q':
			exit(EXIT_SUCCESS);
	}

	glFlush();
}

int is_in_model(float x0, float y0, float z0)
{
	int i;

	for (i = 1 ; i < num_poligons ; ++i)
		if ((x0 * coefs[i].a + y0 * coefs[i].b + z0 * coefs[i].c + coefs[i].d) > 0)
			return 0;

	return 1;
}

int main(int argc, char **argv)
{
	char mod, *t;
	float x, y, z;
	char buffer[256];
	FILE *in;
	int p1, p2, p3;
	float x0, y0, z0;
		
	if (argc != 2) {
		fprintf(stderr, "Sintaksa: object [obj fajl]\n");
		exit(EXIT_FAILURE);
	}
	
	if ((in = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Ne moze se otvoriti: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	printf("Unesi tocku: ");
	scanf("%f %f %f", &x0, &y0, &z0);
	
	num_points = num_poligons = 0;
	
	while(fgets(buffer, 256, in)) {
		t = strchr(buffer, '\n');
		*t = '\0';
		if (strchr(buffer, '#'))
			continue;

		if (sscanf(buffer, "%c %f %f %f", &mod, &x, &y, &z) == 4) {
			switch (mod) {
				case 'v':
				case 'V':
					++num_points;
					points[num_points].x = x;
					points[num_points].y = y;
					points[num_points].z = z;
					break;
				case 'f':
				case 'F':
					++num_poligons;
					p1 = poligons[num_poligons].v1 = (int) x;
					p2 = poligons[num_poligons].v2 = (int) y;
					p3 = poligons[num_poligons].v3 = (int) z;

					coefs[num_poligons].a = (points[p2].y - points[p1].y) * (points[p3].z - points[p1].z)
							- (points[p2].z - points[p1].z) * (points[p3].y - points[p1].y);
					coefs[num_poligons].b = -(points[p2].x - points[p1].x) * (points[p3].z - points[p1].z)
							+ (points[p2].z - points[p1].z) * (points[p3].x - points[p1].x);
					coefs[num_poligons].c = (points[p2].x - points[p1].x) * (points[p3].y - points[p1].y)
							- (points[p2].y - points[p1].y) * (points[p3].x - points[p1].x);
					coefs[num_poligons].d = -points[p1].x * coefs[num_poligons].a -
							points[p1].y * coefs[num_poligons].b - points[p1].z * coefs[num_poligons].c;

					break;
			}
		}
	}

	if (is_in_model(x0, y0, z0))
		printf ("Tocka je unutar tijela\n");
	else
		printf ("Tocka je izvan tijela\n");
	
	glutInit(&argc, argv);
	init_gl(); 
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutCreateWindow(argv[0]);
	
	glutDisplayFunc(draw_scene);
	glutIdleFunc(draw_scene);
	glutReshapeFunc(resize_scene);
	glutKeyboardFunc(keyboard_handler);

	glutMainLoop();
	
	return EXIT_SUCCESS;
}
