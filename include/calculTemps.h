#ifndef CALCULTEMPS_H
#define CALCULTEMPS_H
#include <stdio.h>
#include <time.h>
struct timespec soustraction(struct timespec *debut, struct timespec *fin);
void printTimespec(struct timespec *t);
int getTimeError(struct timespec *temps);
#endif
