#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../include/calculTemps.h"
#define MULTIPLE (int) 10

void multiplication_mat(int * ma, int * mb, int * mc, int taille);
void affiche_mat(int * ma, int taille);


int main(int argc, char const *argv[])
{
	int *A, *B, *C;
	int taille_matrice, nb_thread;
	int i, j, count = 0;

	if(argc < 3)
	{
		printf("Vous devez spécifier le nombre de thread max à utiliser et la taille de matrice multiple de 100.\n");
		printf("Exemple : ./openmp.exe 4 100\n");
		return 1;
	}
	omp_set_num_threads(atoi(argv[1]));

	taille_matrice = atoi(argv[2]);

	A = (int*) malloc(sizeof(int) * (taille_matrice * taille_matrice));
	B = (int*) malloc(sizeof(int) * (taille_matrice * taille_matrice));
	C = (int*) malloc(sizeof(int) * (taille_matrice * taille_matrice));

	/*Debut du timer*/
	if (getTimeDebut())
	{
		exit(1);
	}
	/* Initialisation de la matrice */
	#pragma omp parallel private(i,j) shared(taille_matrice, A, B, C)
	{
		#pragma omp for schedule(dynamic,2)
		for(i = 0; i < taille_matrice; ++i)
		{
			for(j = 0; j < taille_matrice; ++j)
			{
				A[(i*taille_matrice) + j] = count++;
				B[(i*taille_matrice) + j] = count++;
			}
		}

		multiplication_mat(A, B, C, taille_matrice);
	}
	/*Fin du timer*/
	if (getTimeFin())
	{
		exit(1);
	}
	/*Affichage du temps ecoule*/
	printTimeRes();

	return 0;
}


void multiplication_mat(int * ma, int * mb, int * mc, int taille)
{
	int i, k, l, res;

	#pragma omp for schedule(dynamic,2)
	for (k = 0; k < taille; ++k)
	{
		for (l = 0; l < taille; ++l)
		{
			res = 0;
			for (i = 0; i < taille; ++i)
			{
				res += ma[(k * taille) + i] * mb[(i * taille) + l];
			}
			mc[(k * taille) + l] = res;
		}
	}
}

void affiche_mat(int * ma, int taille)
{
	int i, j;

	for(i = 0; i < taille; ++i)
	{
		for(j = 0; j < taille; ++j)
		{
			printf("%3d ", ma[(i * taille) + j]);
		}
		printf("\n");
	}
}

