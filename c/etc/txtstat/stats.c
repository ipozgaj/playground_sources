#include "stats.h"

void analyze (FILE *in)
{
	int ch;
	int i, j;

	i = j = 0;
		
	while ((ch = fgetc (in)) != EOF) {
		++total;
		++i, ++j;      /* word and sentences lenght counter */
		
		if (ch == '.') {
		/* add sentence length to total sum and reset counter. increase sentece count */
			sent_sum += j;
			j = 0;
			++dots;
		}
		
		if (ch == '\n') ++lines;
		if (isspace (ch)) {
			/* add word length to total sum and reset counter. increase word count*/
			word_sum += i;
			i = 0;
			++words;
		}
		
		if (isalpha(ch)) {
			if (ch >= 'A' && ch <= 'Z') {
				++characters[ch-'A'];
				++chars;	
			}
			
			if (ch >= 'a' && ch <= 'z') {
				++characters[ch-'a'];
				 ++chars;
			}
		}
	}

	/* Fix initial size of words, lines and sentences (no SPACE or \n character) */
	if (total != 0 && words == 0) ++words;
	if (total != 0 && lines == 0) ++lines;
	if (total != 0 && dots == 0) ++dots;
	if (total != 0 && sent_sum == 0) sent_sum += total;
	if (total != 0 && word_sum == 0) word_sum += total;				 
}

void display_graph (FILE *out)
{
	int i, j;
	
	/*  FIXME: if input is only one character long, the output will be longer than screen width */
	
	for (i = 0 ; i <= 'Z'-'A' ; ++i) {
		fprintf (out, "%c %-7d", 'a' + i, characters[i]);
		for (j = 0 ; j < (int)((float) characters[i] / chars * 100) ; ++j)
			fprintf (out, "*");

	fprintf (out, "\n");											    }
}

void display_stats (FILE *out)
{
	fprintf (out, "\nTotal: %d characters, %d words, %d lines\n", total, words, lines);
	fprintf (out, "%d letters of total %d characters (%4.1f%)\n", chars, total, (float) chars/total*100);
	fprintf (out, "Average word length: %d (%3.1f)\n", word_sum/words, (float) word_sum/words);
	fprintf (out, "%d sentences, average length: %d (%3.1f)\n", dots, sent_sum/dots, (float) sent_sum/dots);
}
