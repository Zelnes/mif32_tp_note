#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/calculTemps.h"

#define LIGNE	1
#define COLONNE 0

//-------------------------------------------------------------------
// Fonctions utilitaires
void multiplication_mat(int * ma, int * mb, int * mc, int taille);
void affiche_mat(int * ma, int taille);
int calculCount(int *coords, int taille_block, int taille_cart);
void additionMatrice(int *res, int *add, int taille);

void initialiseMonde(int argc, char** argv);
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Variables globales
int size, rank, rankCart, rankRow, rankCol, racine;
int coords[2], tab_dim[2];
MPI_Comm COMM_CART, COMM_ROWS, COMM_COLS;
int taille_matrice, taille_block, square_block;
//-------------------------------------------------------------------


int main (int argc, char**argv)
{
	int i, j, k, l, count;
	int * A, * B, * C, * A1, * B1, *C1, * tmpB;
	int dest, src;

	// Initialisation du monde
	initialiseMonde(argc, argv);

	square_block = taille_block * taille_block;

	//Allocations des matrices
	A  = (int*) malloc(sizeof(int) * (square_block));
	B  = (int*) malloc(sizeof(int) * (square_block));
	C  = (int*) malloc(sizeof(int) * (square_block));
	A1 = (int*) malloc(sizeof(int) * (square_block));
	B1 = (int*) malloc(sizeof(int) * (square_block));
	C1 = (int*) malloc(sizeof(int) * (square_block));

	if(rank == 0)
	{
		/*Debut du timer*/
		if (getTimeDebut())
		{
			MPI_Finalize();
			exit(1);
		}
	}

	//Initialisation de la matrice de résultat
	for (i = 0; i < square_block; i++)
	{
		C[i] = 0;
	}

	//Génération des matrices
	count = calculCount(coords, taille_block, sqrt(size));
	#pragma omp for schedule(dynamic,2)
		for (k = 0; k < taille_block; ++k)
		{
			for (l = 0; l < taille_block; ++l)
			{
				A[(k * taille_block) + l]  = count++;
				B[(k * taille_block) + l]  = count++;
			}
			count += 2 * (taille_matrice - taille_block);
		}

	for (i = 0; i < racine; i++)
	{
		if ((coords[0] + i) % racine == rankRow)
		{
			MPI_Bcast(A, square_block, MPI_INT, (coords[0] + i) % racine, COMM_ROWS);
			multiplication_mat(A, B, C1, taille_block);
		}
		else
		{
			MPI_Bcast(A1, square_block, MPI_INT, (coords[0] + i) % racine, COMM_ROWS);
			multiplication_mat(A1, B, C1, taille_block);
			//Reception
		}
		additionMatrice(C, C1, square_block);

		if(rankCol == 0)
			dest = racine - 1;
		else
			dest = rankCol - 1;

		src = (rankCol + 1) % racine;

		if(rankCol % 2 == 0)
		{
			MPI_Send(B,  square_block, MPI_INT, dest, 0, COMM_COLS);
			MPI_Recv(B1, square_block, MPI_INT, src,  0, COMM_COLS, MPI_STATUS_IGNORE);
		}
		else
		{
			MPI_Recv(B1, square_block, MPI_INT, src,  0, COMM_COLS, MPI_STATUS_IGNORE);
			MPI_Send(B,  square_block, MPI_INT, dest, 0, COMM_COLS);
		}

		// Copie de B1 dans B
		tmpB = B;
		B = B1;
		B1 = tmpB;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0)
	{
		/*Debut du timer*/
		if (getTimeFin())
		{
			MPI_Finalize();
			exit(1);
		}

		printTimeRes();
	}

	// for(i = 0; i < size; i++)
	// {
	// 	MPI_Barrier(COMM_CART);
	// 	if (i == rank)
	// 	{
	// 		affiche_mat(C, taille_block);
	// 	}
	// }

	free(A);
	free(B);
	free(C);
	free(A1);
	free(B1);
	free(C1);
	MPI_Finalize();

	return 0;
}

void initialiseMonde(int argc, char** argv)
{
	int remain[2], periods[2], reorder, i, nb_thread;

	MPI_Init (&argc, &argv);      				  /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */

	// On vérifie qu'il existe au moins un deuxieme argument 
	if(argc < 3)
	{
		MPI_Finalize();
		if(rank == 0)
			printf("Le nombre d'arguments n'est pas suffisant\nAssurez vous de préciser en premier argument le nombre de thread puis la taille de matrice");
		exit(1);
	}
	nb_thread = atoi(argv[1]);
	omp_set_num_threads(nb_thread);

	taille_matrice = atoi(argv[2]);

	racine = sqrt(size);
	if (racine * racine != size || taille_matrice % racine != 0)
	{
		MPI_Finalize();
		if(rank == 0)
			printf("Le nombre de processus MPI n'est pas cohérent avec la taille de la matrice\n");
		exit(1);
	}

	taille_block = taille_matrice / racine;

	tab_dim[LIGNE]   = racine;
	tab_dim[COLONNE] = racine;
	periods[LIGNE]   = 0;
	periods[COLONNE] = 0;
	reorder = 0;

	MPI_Cart_create(MPI_COMM_WORLD, 2, tab_dim, periods, reorder, &COMM_CART);
	// Récupération du rang dans le communicateur cartésien
	MPI_Comm_rank(COMM_CART, &rankCart);

	// Récupération des coordonnées dans le communicateur cartésien
	MPI_Cart_coords(COMM_CART, rankCart, 2, coords);

	// Création du communicateur en ligne
	remain[LIGNE]   = 1;
	remain[COLONNE] = 0;
	MPI_Cart_sub(COMM_CART, remain, &COMM_ROWS);
	// Récupération du rang dans le communicateur en ligne
	MPI_Comm_rank(COMM_ROWS, &rankRow);

	// Création du communicateur en colonne
	remain[LIGNE]   = 0;
	remain[COLONNE] = 1;
	MPI_Cart_sub(COMM_CART, remain, &COMM_COLS);
	// Récupération du rang dans le communicateur en colonne
	MPI_Comm_rank(COMM_COLS, &rankCol);
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

	for (i = 0; i < taille; ++i)
	{
		for (j = 0; j < taille; ++j)
		{
			printf("%3d ", ma[(i * taille) + j]);
		}
		printf("\n");
	}
}


int calculCount(int *coords, int taille_block, int taille_cart)
{
	return 2 * (coords[0] * taille_block * taille_block * taille_cart + coords[1] * taille_block);
}

void additionMatrice(int *res, int *add, int taille)
{
	int i;

	#pragma omp for schedule(dynamic,2)
	for (i = 0; i < taille; i++)
	{
		res[i] += add[i];
	}
}
