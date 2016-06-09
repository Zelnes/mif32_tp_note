#include "../include/calculTemps.h"
int getTimeError(struct timespec *temps)
{
	if(clock_gettime(CLOCK_MONOTONIC, temps)) 
	{ 
		perror("Probleme de clock_gettime !\n");
		return 1;
	}
	return 0;
}

void printTimespec(struct timespec *t)
{
	printf("Temps écoulé : ");
	if(t->tv_nsec<0)
	{
		printf("-%lld.%.9ld", (long long)t->tv_sec, -t->tv_nsec);
	}
	else
	{
		printf("%lld.%.9ld", (long long)t->tv_sec, t->tv_nsec);
	}
	printf(" seconde(s)\n");
}

struct timespec soustraction(struct timespec *debut, struct timespec *fin)
{
	struct timespec resultat; 
	if (fin->tv_nsec > debut->tv_nsec) 
	{
		if(fin->tv_sec >= debut->tv_sec)
		{		
			resultat.tv_sec = fin->tv_sec - debut->tv_sec;
			resultat.tv_nsec = fin->tv_nsec - debut->tv_nsec;
		}
		else
		{
			resultat.tv_sec = debut->tv_sec - fin->tv_sec - 1;
			resultat.tv_nsec = -(1000000000 + debut->tv_nsec - fin->tv_nsec);	
		}
	} 
	else if(fin->tv_nsec == debut->tv_nsec)
	{
		resultat.tv_nsec = 0;
		if(fin->tv_sec == debut->tv_sec)
		{
			resultat.tv_sec = 0;
		}
		else
		{
			resultat.tv_sec = fin->tv_sec - debut->tv_sec;
		}
	} 
	else 
	{
		if(fin->tv_sec > debut->tv_sec)
		{
			resultat.tv_sec = fin->tv_sec - debut->tv_sec - 1;
			resultat.tv_nsec = 1000000000 + fin->tv_nsec - debut->tv_nsec;
		}
		else if (fin->tv_sec < debut->tv_sec)
		{
			resultat.tv_sec = -(debut->tv_sec - fin->tv_sec);
			resultat.tv_nsec = debut->tv_nsec - fin->tv_nsec;
		}
		else
		{
			resultat.tv_sec =  0;
			resultat.tv_nsec = -(debut->tv_nsec - fin->tv_nsec);
		}
	}
	return resultat;
}
