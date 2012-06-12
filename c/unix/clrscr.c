/* clrscr.c - clear screen program (conforming to ANSI X3.64 standard)
 *
 * (C) 2004  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 *
 * \033[2J    - erase the screen
 * \033[1;1H  - position the cursor to row 1 column 1
 */

#include <stdio.h>

int main()
{
	printf("\033[2J\033[1;1H");
		
	return 0;
}
