#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "mpi.h"

/* zajednicke konstante */
#define MAX_PAUZA 2
#define MAX_ZAHTJEV 4
#define BROJ_SJEDALA 100

#define TAG_ZAHTJEV	10
#define TAG_ODGOVOR 11

/* vrsta poruka koje master salje slave procesima */
#define MSG_NASAO_MJESTO  0
#define MSG_NEMA_MJESTA   1	
#define MSG_ZAVRSI        2

/* vraca 0 ako su sva potrebna mjesta slobodna, inace 1 */
int zauzeto(int *mjesta, int pocetno, int duljina)
{
	int i;

	for (i = pocetno ; i < pocetno + duljina ; ++i)
		if (mjesta[i])
			return 1;
	
	return 0;
}

/* rezervira mjesta u dvorani za odredjenu blagajnu */
void zauzmi_mjesta(int *mjesta, int blagajna, int pocetno, int duljina)
{
	int i;

	for (i = pocetno ; i < pocetno + duljina ; ++i)
		mjesta[i] = blagajna;
}


int main(int argc, char **argv)
{
	int broj_procesa, id_procesa;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &broj_procesa);
	MPI_Comm_rank(MPI_COMM_WORLD, &id_procesa);
	
	/* *id_procesa ide zato da svaki proces dobije jedinstveni seed jer 
	 * inace ima problema ako se svih n procesa pokrene u istoj sekundi */
	srand((unsigned) time(NULL) * id_procesa);


	if (id_procesa == 0) {
		/* master proces */
		int poruka[2];
		int mjesta[BROJ_SJEDALA] = {0};
		int broj_zauzetih = 0, ok;
		int i, pocetno_mjesto, broj_karata;

		while(1) {
			MPI_Recv(poruka, 2, MPI_INT, MPI_ANY_SOURCE, TAG_ZAHTJEV, MPI_COMM_WORLD, &status);
			
			fflush(stdout);

			pocetno_mjesto = poruka[0];
			broj_karata = poruka[1];

			ok = 0;

			if (!zauzeto(mjesta, pocetno_mjesto, broj_karata)) {
				/* obrada pocetnog zahtjeva */
				poruka[0] = MSG_NASAO_MJESTO;
				poruka[1] = pocetno_mjesto;
				ok = 1;
				MPI_Send(poruka, 2, MPI_INT, status.MPI_SOURCE, TAG_ODGOVOR, MPI_COMM_WORLD);
			} else {
				/* gledaj susjedna mjesta od pocetnog zahtjeva s obje strane */
				for (i = 0 ; i < broj_karata ; ++i)
					if (!zauzeto(mjesta, pocetno_mjesto + i, broj_karata)) {
						poruka[0] = MSG_NASAO_MJESTO;
						poruka[1] = pocetno_mjesto + i;
						ok = 1;
						MPI_Send(poruka, 2, MPI_INT, status.MPI_SOURCE, TAG_ODGOVOR, MPI_COMM_WORLD);
						break;
					} else if (!zauzeto(mjesta, pocetno_mjesto - i, broj_karata)) {
						poruka[0] = MSG_NASAO_MJESTO;
						poruka[1] = pocetno_mjesto - i;
						ok = 1;
						MPI_Send(poruka, 2, MPI_INT, status.MPI_SOURCE, TAG_ODGOVOR, MPI_COMM_WORLD);
						break;
					}
			}

			if (ok) {
				/* zauzmi mjesta i provjeri da li smo gotovi */
				broj_zauzetih += broj_karata;
				zauzmi_mjesta(mjesta, status.MPI_SOURCE, poruka[1], broj_karata);

				if (broj_zauzetih >= 90) {
					printf("\nProdano 90 posto ulaznica\n");
					printf("--------------------------\n");
					for (i = 0 ; i < BROJ_SJEDALA ; ++i)
						if (i % 10)
							if (mjesta[i] == 0)
								printf("*  ");
							else
								printf("%d  ", mjesta[i]);
						else if (i)
							printf("\n");
					printf("\n");
					fflush(stdout);

					/* signaliziraj svima da nema vise karata i izadji */
					poruka[0] = MSG_ZAVRSI;
					
					//MPI_Bcast(poruka, 2, MPI_INT, id_procesa, MPI_COMM_WORLD);
					for (i = 1 ; i < broj_procesa ; ++i)
						MPI_Send(poruka, 2, MPI_INT, i, TAG_ODGOVOR, MPI_COMM_WORLD);

					MPI_Finalize();
					return 0;
				}
			} else {
				/* Nismo nasli slobodna mjesta za trenutni zahtjev */
				poruka[0] = MSG_NEMA_MJESTA;
				MPI_Send(poruka, 2, MPI_INT, status.MPI_SOURCE, TAG_ODGOVOR, MPI_COMM_WORLD);
			}
		}
	} else {
		/* slave procesi */
		int i, pauza, broj_mjesta, pocetno_mjesto, poruka[2];

		while(1) {
			pauza = rand() % (MAX_PAUZA + 1);
			broj_mjesta = 1 + rand() % MAX_ZAHTJEV;

			/* pazi da npr ne zahtjeva pocetno mjesto 2 a 4 ulaznice! */ 
			pocetno_mjesto = broj_mjesta + (rand() % (BROJ_SJEDALA - 2 * broj_mjesta));
				
			/* prazni izlazni buffer jer se inace nista nece ispisati */
			fflush(stdout);

			sleep(pauza);

			poruka[0] = pocetno_mjesto;
			poruka[1] = broj_mjesta;
			MPI_Send(poruka, 2, MPI_INT, 0, TAG_ZAHTJEV, MPI_COMM_WORLD);
			MPI_Recv(poruka, 2, MPI_INT, 0, TAG_ODGOVOR, MPI_COMM_WORLD, &status);

			if (poruka[0] == MSG_NASAO_MJESTO) {
				printf("Blagajna %d zauzela mjesta: ", id_procesa);

				for (i = poruka[1] ; i < poruka[1] + broj_mjesta ; ++i)
					printf("%d ", i);
				printf("\n\n");
			}

			if (poruka[0] == MSG_ZAVRSI) {
				MPI_Finalize();
				return 0;
			}
		}
	}

	MPI_Finalize();

	return 0;
}
