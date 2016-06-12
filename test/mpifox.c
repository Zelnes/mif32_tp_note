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
int taille_matrice, taille_block;
//-------------------------------------------------------------------


int main (int argc, char**argv)
{
	int i, j, k, l, count;
	int * A, * B, * C, * A1, * B1, *C1, * tmpB;
	int dest, src;

	// Initialisation du monde
	initialiseMonde(argc, argv);

	//Allocations des matrices
	A  = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	B  = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	C  = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	A1 = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	B1 = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	C1 = (int*) malloc(sizeof(int) * (taille_block * taille_block));

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
	for (i = 0; i < taille_block * taille_block; i++)
	{
		C[i] = 0;
	}


	//Génération des matrices
	count = calculCount(coords, taille_block, sqrt(size));
	for (k = 0; k < taille_block; ++k)
	{
		for (l = 0; l < taille_block; ++l)
		{
			A[(k * taille_block) + l]  = count++;
			B[(k * taille_block) + l]  = count++;
		}
		count += 2 * (taille_matrice - taille_block);
	}

	//Affichage des matrices générées
	/*for(i = 0; i < size; i++)
	{
		MPI_Barrier(COMM_CART);
		if (i == rank)
		{
			affiche_mat(A,taille_block);
			affiche_mat(B,taille_block);
		}
	}*/

	for (i = 0; i < racine; i++)
	{
		MPI_Barrier(MPI_COMM_WORLD);
		printf("[%d] Debut boucle %d\n", rank, i);
		//Diffusion diagonale de A dans A1
		printf("[%d] Bcast a partir de %d en ligne %d\n", rank, (coords[0] + i) % racine, rankRow);
		if ((coords[0] + i) % racine == rankRow)
		{
			MPI_Bcast(A, taille_block * taille_block, MPI_INT, (coords[0] + i) % racine, COMM_ROWS);
			multiplication_mat(A, B, C1, taille_block);
		}
		else
		{
			MPI_Bcast(A1, taille_block * taille_block, MPI_INT, (coords[0] + i) % racine, COMM_ROWS);
			multiplication_mat(A1, B, C1, taille_block);
			//Reception
		}
		additionMatrice(C, C1, taille_block);

		if(rankCol == 0)
			dest = racine - 1;
		else
			dest = rankCol - 1;

		src = (rankCol + 1) % racine;

		if(rankCol % 2 == 0)
		{
			MPI_Send(B,  taille_block * taille_block, MPI_INT, dest, 0, COMM_COLS);
			MPI_Recv(B1, taille_block * taille_block, MPI_INT, src,  0, COMM_COLS, MPI_STATUS_IGNORE);
		}
		else
		{
			MPI_Recv(B1, taille_block * taille_block, MPI_INT, src,  0, COMM_COLS, MPI_STATUS_IGNORE);
			MPI_Send(B,  taille_block * taille_block, MPI_INT, dest, 0, COMM_COLS);
		}

		// Copie de B1 dans B
		tmpB = B;
		B = B1;
		B1 = tmpB;

		//Shift de B dans B1
		// if(rankCol == 0)
		// {
		// 	printf("[%d] Avant send pour %d\n", rank, racine - 1);
		// }
		// else
		// {
		// 	printf("[%d] Avant send pour %d\n", rank, rankCol - 1);
		// 	MPI_Send(B, taille_block * taille_block, MPI_INT, rankCol - 1, 0, COMM_COLS);
		// 	printf("[%d] Après send\n", rank);
		// 	MPI_Recv(B, taille_block * taille_block, MPI_INT, (rankCol + 1) % racine, 0, COMM_COLS, MPI_STATUS_IGNORE);
		// }

		MPI_Barrier(MPI_COMM_WORLD);
		printf("[%d] Fin boucle %d\n", rank, i);
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

	for(i = 0; i < size; i++)
	{
		MPI_Barrier(COMM_CART);
		if (i == rank)
		{
			affiche_mat(C, taille_block);
		}
	}

	//printf("[%d]\t->\t[%d,%d] %d\n", rank, coords[LIGNE], coords[COLONNE], i);
	free(A);
	free(B);
	free(C);
	free(A1);
	free(B1);
	free(C1);
	MPI_Finalize();


	if(rank == 0)
		printf("Fin\n");

	return 0;
}

void initialiseMonde(int argc, char** argv)
{
	int remain[2], periods[2], reorder, i;

	MPI_Init (&argc, &argv);      				  /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */

	// On vérifie qu'il existe au moins un deuxieme argument 
	if(argc < 2)
	{
		MPI_Finalize();
		exit(1);
	}

	taille_matrice = atoi(argv[1]);

	racine = sqrt(size);
	if (racine * racine != size || taille_matrice % size != 0)
	// if (size != 4 || taille_matrice % 4 != 0)
	{
		MPI_Finalize();
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

	for(i = 0; i < size; i++)
	{
		MPI_Barrier(COMM_CART);
		if (i == rank)
		{
			printf("[%d]:[%d]:[%d]:[%d]\t->\t[%d,%d]\n", rank, rankCart, rankRow, rankCol, coords[0], coords[1]);
		}
	}
}




void multiplication_mat(int * ma, int * mb, int * mc, int taille)
{
	int i, k, l, res;
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
	for (i = 0; i < taille; i++)
	{
		res[i] += add[i];
	}
}
