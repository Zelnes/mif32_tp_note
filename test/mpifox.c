#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LIGNE	1
#define COLONNE 0
void multiplication_mat(int * ma, int * mb, int * mc, int taille);
void affiche_mat(int * ma, int taille);
int calculCount(int *coords, int taille_block, int taille_cart);
void additionMatrice(int *res, int *add, int taille)

int main (int argc, char**argv)
{
	int rank, size, i, j, k, l, count;
	int tmp, tab_dim[2], reorder, coords[2], coordsBcastSrc[2];
	int periods[2] = {0,0};
	int remain[2] = {1,0};
	int * A, * B, * C, * A1, *B1, *C1, taille_matrice, taille_block;
	int rankBcastSrc;
	int coords_actuel[2],root;
	MPI_Comm COMM_CART;

	MPI_Init (&argc, &argv);      /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */

	taille_matrice = atoi(argv[1]);
	//printf("taille_mat %d\n",taille_matrice);
	
	if(size!=4 || taille_matrice%4!=0)
	{
		MPI_Finalize();
		return 1;
	}
	taille_block = taille_matrice/sqrt(size);
	//Allocations des matrices
	A = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	B = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	C = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	A1 = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	B1 = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	C1 = (int*) malloc(sizeof(int) * (taille_block * taille_block));
	//Initialisation de la matrice de résultat
	for(i=0;i<taille_block * taille_block;i++)
	{
		C[i] = 0;
	}
	
	tab_dim[LIGNE] = sqrt(size);
	tab_dim[COLONNE] = sqrt(size);
	reorder = 0;

	MPI_Cart_create (MPI_COMM_WORLD, 2, tab_dim, periods, reorder, &COMM_CART);
	MPI_Cart_coords (COMM_CART, rank, 2, coords);
	for(i = 0; i < size; i++) 
	{
		MPI_Barrier(COMM_CART);
		if (i == rank) 
		{
			printf(" rank %d\t->\t[%d,%d]\n", rank, coords[LIGNE], coords[COLONNE]);
		}
	}
	//Génération des matrices
	for(i=0;i<tab_dim[LIGNE];i++)
	{
		for(j=0;j<tab_dim[COLONNE];j++)
		{
			if(coords[LIGNE]==i && coords[COLONNE]==j)
			{
				count = calculCount(coords, taille_block, sqrt(size));
				for(k = 0; k < taille_block; ++k)
				{
					for(l = 0; l < taille_block; ++l)
					{
						A[(k*taille_block) + l] = count++;
						B[(k*taille_block) + l] = count++;
					}
					count+=2*(taille_matrice-taille_block);
				}
			}
		}
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
	
	for(i=0;i<sqrt(size);i++)
	{
		//Diffusion diagonale de A dans A1
		
		//Shift de B dans B1
		
		//multiplication_mat(A1, B1, C1, taille_block);
		//additionMatrice(C, C1, taille_block);
	}


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


int calculCount(int *coords, int taille_block, int taille_cart)
{
	return 2*(coords[0]*taille_block*taille_block*taille_cart + coords[1]*taille_block);
}

void additionMatrice(int *res, int *add, int taille)
{
	int i;
	for(i=0;i<taille;i++)
	{
		res[i] += add[i]; 
	}
}
