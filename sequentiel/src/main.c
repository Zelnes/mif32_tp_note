#include <stdio.h>
#include <stdlib.h>

/* Matrice utilisée dans le programme */
int * matrice;
/* Taille de la matrice utilisée dans le programme */
int taille_matrice;


void multiplication_mat(int * ma, int * mb, int * mc, int rows, int columns);
void affiche_mat()

int main(int argc, char const *argv[])
{
	int i, j, count = 0;

	if(argc < 2)
	{
		printf("Vous devez spécifier au moins une taille de matrice multiple de 100.\n");
		return 1;
	}

	taille_matrice = atoi(argv[1]);
	printf("taille_matrice : %d\n", taille_matrice);

	if((taille_matrice % 100) != 0)
	{
		printf("Vous devez spécifier une taille de matrice multiple de 100.\n");
		return 1;
	}

	matrice = (int*) malloc(sizeof(int) * (taille_matrice * taille_matrice));

	/* Initialisation de la matrice */
	for(i = 0; i < taille_matrice; ++i)
	{
		for(j = 0; j < taille_matrice; ++j)
		{
			matrice[(i*taille_matrice) + j] = count++;
		}
	}


	return 0;
}


void multiplication_mat(int * ma, int * mb, int * mc, int rows, int columns)
{
	int i, j, k, l, res, cpt = 0;

	for (k = 0; k < rows; ++k)
	{
		for (l = 0; l < columns; ++l)
		{
			for (i = 0; i < rows; ++i)
			{
				res = 0;

				for (j = 0; j < columns; ++j)
				{
					res += ma[(k * columns) + j] * mb[(l * rows) + i];
				}
				mc[cpt++] = res;
			}
			
		}
	}
}