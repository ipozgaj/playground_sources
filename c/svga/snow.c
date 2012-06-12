/* snow.c - snow animation using svgalib
 * (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vga.h>

#define MODE G640x480x256
#define MAXX    640
#define MAXY    480
#define COLOR   100
#define DENSITY 200			/* lower value is higher density */

char buffer[MAXX][MAXY];

void delete_pixel (int x, int y)
{
	vga_setcolor (0);
	vga_drawpixel (x, y);
	buffer[x][y] = 0;
	vga_setcolor (COLOR);
}

void init ()
{
	int i, j;
	if (vga_init ()) {
		fprintf (stderr, "Could not initialize SVGA system\n");
		exit (EXIT_FAILURE);
	};
	
	vga_setmode (MODE);
	vga_setcolor (COLOR);

	srand (time ((unsigned) NULL));
	
	for (i=0 ; i<MAXX ; i++)
		for (j=0 ; j<MAXY ; j++)
			buffer[i][j] = 0;
}

int main ()
{	
	int x, y, i;
	int direction;
	
	init ();
	
	while (1) {
		for (y=MAXY-2 ; y>=0 ; y--)
			for (x=0 ; x<MAXX ; x++)
				if (buffer[x][y]) {
					/* Chose the move direction */
					direction = rand () % 2;

					if (direction) {
						/* Try to move pixel to left */
						if (!buffer[x-1][y+1] && x>0) {
							delete_pixel (x, y);
							vga_drawpixel (x-1, y+1);
							buffer[x-1][y+1] = 1;
						} else if (!buffer[x+1][y+1] && x<MAXX-1) {
							delete_pixel (x, y);
							vga_drawpixel (x+1, y+1);
							buffer[x+1][y+1] = 1;
						} else if (!buffer[x][y+1]) {
							delete_pixel (x, y);
							vga_drawpixel (x, y+1);
							buffer[x][y+1] = 1;
						}
					} else {
						/* Move right */
						if (!buffer[x+1][y+1] && x<MAXX-1) {
							delete_pixel (x, y);
							vga_drawpixel (x+1, y+1);
							buffer[x+1][y+1] = 1;
						} else if (!buffer[x-1][y+1] && x>0) {
							delete_pixel (x, y);
							vga_drawpixel (x-1, y+1);
							buffer[x-1][y+1] = 1;
						} else if (!buffer[x][y+1]) {
							delete_pixel (x, y);
							vga_drawpixel (x, y+1);
							buffer[x][y+1] = 1;
						}
					}
				}

		vga_waitretrace();

		for (i=0 ; i<MAXX ; i++)
				if ((rand() % DENSITY) == 0) {
						vga_drawpixel (i, 0);
						buffer[i][0] = 1;
				}
	}
	
	return 0;
}
