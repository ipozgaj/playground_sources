#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef __linux__
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   768
#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480
#define WINDOW_POS_X    ((SCREEN_WIDTH - WINDOW_WIDTH) / 2)
#define WINDOW_POS_Y    ((SCREEN_HEIGHT - WINDOW_HEIGHT) / 2)

typedef struct {
	float x, y, z;
} tocka_t;

int window_width = WINDOW_WIDTH, window_height = WINDOW_HEIGHT;
int fullscreen;
int i, broj_vrhova;
tocka_t ociste, glediste, g1, g2, *vrhovi = NULL, *novi_vrhovi, vrh_t, vrh_p;
float t1[4][4] = {0}, t2[4][4] = {0}, t3[4][4] = {0}, t4[4][4] = {0}, t5[4][4] = {0}, tmp[4][4], t[4][4];
float p[4][4] = {0};
float sinus, kosinus;

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
 //   gluOrtho2D(0, window_width, 0, window_height);
    gluOrtho2D(-5, 5, -5, 5);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1);
    glColor3f(0, 1, 0);
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
}


void mouse_handler(int button, int state, int x, int y)
{
	int i;
	
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		glBegin(GL_LINES);
		for (i = 0 ; i < broj_vrhova ; ++i) {
			glVertex2f(novi_vrhovi[i].x, novi_vrhovi[i].y);
			glVertex2f(novi_vrhovi[i+1].x, novi_vrhovi[i+1].y);
		}
		glVertex2f(novi_vrhovi[0].x, novi_vrhovi[0].y);
		glVertex2f(novi_vrhovi[broj_vrhova-1].x, novi_vrhovi[broj_vrhova-1].y);
		glEnd();
        glFlush();
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        resize_scene(window_width, window_height);
    }
}

/* radi samo na 4x4 matricama */
void mnozi_matrice(float *a, float *b, float *rez)
{
	int i, j, k;
	double zbroj;

	for (i = 0 ; i < 4 ; ++i)
		for (j = 0 ; j < 4 ; ++j) {
			for (zbroj = 0, k = 0 ; k < 4 ; ++k)
				zbroj += a[i * 4 + k] * b[k * 4 + j];
			rez[i * 4 + j] = zbroj;
		}
}

/* transformacija tocke */
void transformiraj(tocka_t *orig, float *b, tocka_t *r)
{
	int i, j, k;
	float a[4], rez[4];
	double zbroj;

	a[0] = orig->x;
	a[1] = orig->y;
	a[2] = orig->z;
	a[3] = 1;
	
	for (i = 0 ; i < 1 ; ++i)
		for (j = 0 ; j < 4 ; ++j) {
			for (zbroj = 0, k = 0 ; k < 4 ; ++k)
				zbroj += a[i * 4 + k] * b[k * 4 + j];
			rez[i * 4 + j] = zbroj;
		}

	r->x = rez[0] / rez[3];
	r->y = rez[1] / rez[3];
	r->z = rez[2] / rez[3];
}

int main(int argc, char **argv)
{
	FILE *in;
		
	if (argc != 2) {
		fprintf(stderr, "Sintaksa: %s [ulaz]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((in = fopen(argv[1], "r")) == NULL) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
	
	fscanf(in, "O = %f %f %f\n", &ociste.x, &ociste.y, &ociste.z);
	fscanf(in, "G = %f %f %f\n", &glediste.x, &glediste.y, &glediste.z);
	
	broj_vrhova = 0;
	
	while(!feof(in)) {
		++broj_vrhova;
		vrhovi = realloc(vrhovi, sizeof(tocka_t) * broj_vrhova);
		fscanf(in, "%f %f %f\n", &vrhovi[broj_vrhova - 1].x, &vrhovi[broj_vrhova - 1].y, &vrhovi[broj_vrhova - 1].z);
	}
	
	t1[0][0] = t1[1][1] = t1[2][2] = t1[3][3] = 1;
	t1[3][0] = -ociste.x;
	t1[3][1] = -ociste.y;
	t1[3][2] = -ociste.z;
	
	g1.x = glediste.x - ociste.x;
	g1.y = glediste.y - ociste.y;
	g1.z = glediste.z - ociste.z;
	sinus = g1.y / sqrt(pow(g1.x, 2) + pow(g1.y, 2));
	kosinus = g1.x / sqrt(pow(g1.x, 2) + pow(g1.y, 2));
	t2[2][2] = t2[3][3] = 1;
	t2[0][0] = t[1][1] = kosinus;
	t2[0][1] = -sinus;
	t2[1][0] = sinus;

	g2.x = sqrt(pow(g1.x, 2) + pow(g1.y, 2));
	g2.y = 0;
	g2.z = g1.z;
	sinus = g2.x / sqrt(pow(g2.x, 2) + pow(g2.z, 2));
	kosinus = g2.z / sqrt(pow(g2.x, 2) + pow(g2.z, 2));
	t3[1][1] = t3[3][3] = 1;
	t3[0][0] = t3[2][2] = kosinus;
	t3[0][2] = sinus;
	t3[2][0] = -sinus;
	
	t4[1][0] = t4[2][2] = t4[3][3] = 1;
	t4[0][1] = -1;

	t5[1][1] = t5[2][2] = t5[3][3] = 1;
	t5[0][0] = -1;
	
	mnozi_matrice(&t1[0][0], &t2[0][0], &tmp[0][0]);
	mnozi_matrice(&tmp[0][0], &t3[0][0], &t[0][0]);
	mnozi_matrice(&t[0][0], &t4[0][0], &tmp[0][0]);
	mnozi_matrice(&tmp[0][0], &t5[0][0], &t[0][0]);
	
	p[2][3] = 1 / sqrt(pow(g2.x, 2) + pow(g2.z, 2));
	p[0][0] = p[1][1] = 1;
	
	novi_vrhovi = malloc(sizeof(tocka_t) * broj_vrhova);
	
	for (i = 0 ; i < broj_vrhova ; ++i) {
		printf("Tocka %d prije transformacije: [%3.2f %3.2f %3.2f 1]\n", i, vrhovi[i].x, vrhovi[i].y, vrhovi[i].z);

		transformiraj(&vrhovi[i], &t[0][0], &vrh_t);
		printf("Tocka %d nakon transformacije: [%3.2f %3.2f %3.2f 1]\n", i, vrh_t.x, vrh_t.y, vrh_t.z);
		
		transformiraj(&vrh_t, &p[0][0], &vrh_p);
		printf("Tocka %d nakon projekcije:     [%3.2f %3.2f %3.2f 1]\n\n", i, vrh_p.x, vrh_p.y, vrh_p.z);
		novi_vrhovi[i] = vrh_t;
	}
	
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
