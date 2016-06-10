#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define LIGNE	1
#define COLONNE 0
void multiplication_mat(int * ma, int * mb, int * mc, int taille);

int main (int argc, char**argv)
{
	int rank, size, i, j;
	int tmp, tab_dim[2], reorder, coords[2], coordsBcastSrc[2];
	int periods[2] = {0,0};
	int * A, * B, * C, * A1, *B1, *C1, taille_matrice, taille_block;
	int rankBcastSrc;
	MPI_Comm COMM_CART, COMM_ROWS;

	MPI_Init (&argc, &argv);      /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */

	taille_matrice = atoi(argv[1]);
	//printf("%d",taille_matrice);
	
	if(size!=4 || taille_matrice%100!=0)
	{
		MPI_Finalize();
		return 1;
	}
	taille_block = taille_matrice/4;
	A = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	B = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	C = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	A1 = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	B1 = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	C1 = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	
	tab_dim[LIGNE] = 2;
	tab_dim[COLONNE] = 2;
	reorder = 0;

	MPI_Cart_create (MPI_COMM_WORLD, 2, tab_dim, periods, reorder, &COMM_CART);
	MPI_Cart_coords (COMM_CART, rank, 2, coords);
	for(i = 0; i < size; i++) 
	{
		MPI_Barrier(MPI_COMM_WORLD);
		if (i == rank) 
		{
			printf(" rank %d\t->\t[%d,%d]\n", rank, coords[LIGNE], coords[COLONNE]);
		}
	}
	
	/*
	for(i = 0; i < taille_matrice; ++i)
	{
		for(j = 0; j < taille_matrice; ++j)
		{
			A[(i*taille_matrice) + j] = count++;
			B[(i*taille_matrice) + j] = count++;
		}
	}
	
	i = -1;

	/*coordsBcastSrc[LIGNE] = 0;
	coordsBcastSrc[COLONNE] = 0;
	MPI_Cart_rank(COMM_CART, coordsBcastSrc, &rankBcastSrc);
	
	if(rankBcastSrc == rank)
		i = 67;

	MPI_Bcast(&i, 1, MPI_INT, rankBcastSrc, COMM_CART);*/

	//printf("[%d]\t->\t[%d,%d] %d\n", rank, coords[LIGNE], coords[COLONNE], i);


	

	free(A);
	free(B);
	free(C);
	free(A1);
	free(B1);
	free(C1);
	MPI_Finalize();

	return 0;
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
