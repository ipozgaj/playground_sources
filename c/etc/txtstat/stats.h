#ifndef _STATS_H
#define _STATS_H

#ifndef _STDIO_H
#include <stdio.h>
#endif /* _STDIO_H */

#ifndef _STDLIB_H
#include <stdlib.h>
#endif /* _STDLIB_H */

#ifndef _STDLIB_H
#include <ctype.h>
#endif /* _CTYPE_H */

#define ALPHABET_LENGTH ('Z' - 'A' + 1)

int characters[ALPHABET_LENGTH] = {0};
int total=0, chars=0, words=0, lines=0, word_sum=0, dots=0, sent_sum=0;

void analyze (FILE *in);
void display_graph (FILE *out);
void display_stats (FILE *out);

#endif /* _STATS_H */
