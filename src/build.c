#include "rtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

// ----------------------------------------------------
// Funcion que entrega el tiempo transcurrido desde el lanzamiento del
// programa en milisegundos

static int time0= 0;

static int getTime0() {
    struct timeval Timeval;
    gettimeofday(&Timeval, NULL);
    return Timeval.tv_sec*1000+Timeval.tv_usec/1000;
}

void resetTime() {
  time0= getTime0();
}

int getTime() {
  return getTime0()-time0;
}

// ----------------------------------------------------

// Construye los arboles R-Tree para N={2^15,...,2^24} 
// con los metodos Nearest-X y STR para dataset random y europa
// Crea archivo "timeResults.txt" con los tiempos de construccion
void buildTree() {
    int *rndNXtime = (int *)malloc(10*sizeof(int));
    int *euNXtime = (int *)malloc(10*sizeof(int));
    int *rndSTRtime = (int *)malloc(10*sizeof(int));
    int *euSTRtime = (int *)malloc(10*sizeof(int));
    char num = '0';
    for (int i=15; i<25; i++) {
        
		char RandomNX[] = "trees/randomNX";
		char end[] = {num, '.', 'b', 'i', 'n', '\0'};
		strcat(RandomNX, end);
		resetTime();
		int countNXrnd = nearestX(1<<i, "data/random.bin", RandomNX);
		int tiempo_random_nx= getTime();
		rndNXtime[i-15] = tiempo_random_nx;

		char EuropaNX[] = "trees/europaNX";
		char end1[] = {num, '.', 'b', 'i', 'n', '\0'};
		strcat(EuropaNX, end1);
		resetTime();
		int countNXeu = nearestX(1<<i, "data/europa.bin", EuropaNX);
		int tiempo_europa_nx= getTime();
		euNXtime[i-15] = tiempo_europa_nx;

		Entry *entries = (Entry *)malloc((1<<i)*sizeof(Entry));

		char RandomSTR[] = "trees/randomSTR";
		char end2[] = {num, '.', 'b', 'i', 'n', '\0'};
		strcat(RandomSTR, end2);
		resetTime();
		Node *arbolRnd = (Node *)malloc( ((1<<i)/B +1000) *sizeof(Node));
		int countRnd = readPoints("data/random.bin", entries, 1<<i);
		int nodeCountRnd = buildSTR(entries, countRnd, arbolRnd);
		writeTree(RandomSTR, arbolRnd, nodeCountRnd);
		int tiempo_random_str= getTime();
		rndSTRtime[i-15] = tiempo_random_str;
		free(arbolRnd);

		char EuropaSTR[] = "trees/europaSTR";
		char end3[] = {num, '.', 'b', 'i', 'n', '\0'};
		strcat(EuropaSTR, end3);
		resetTime();
		Node *arbolEu = (Node *)malloc( ((1<<i)/B +1000) *sizeof(Node));
		int countRnd1 = readPoints("data/europa.bin", entries, 1<<i);
		int nodeCountRnd1 = buildSTR(entries, countRnd1, arbolEu);
		writeTree(EuropaSTR, arbolEu, nodeCountRnd1);
		int tiempo_europa_str= getTime();
		euSTRtime[i-15] = tiempo_europa_str;
		free(arbolEu);
		free(entries);

		num++;
    }

	FILE *res = fopen("timeResults.txt", "w");
	fprintf(res, "Tiempos dataset random\n");
	fprintf(res, "Nearest-X: [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n",rndNXtime[0],rndNXtime[1],rndNXtime[2],rndNXtime[3],
		rndNXtime[4],rndNXtime[5],rndNXtime[6],rndNXtime[7],rndNXtime[8],rndNXtime[9]);
	fprintf(res, "STR: [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n",rndSTRtime[0],rndSTRtime[1],rndSTRtime[2],rndSTRtime[3],
		rndSTRtime[4],rndSTRtime[5],rndSTRtime[6],rndSTRtime[7],rndSTRtime[8],rndSTRtime[9]);

	fprintf(res, "Tiempos dataset europa\n");
	fprintf(res, "Nearest-X: [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n",euNXtime[0],euNXtime[1],euNXtime[2],euNXtime[3],
		euNXtime[4],euNXtime[5],euNXtime[6],euNXtime[7],euNXtime[8],euNXtime[9]);
	fprintf(res, "STR: [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n",euSTRtime[0],euSTRtime[1],euSTRtime[2],euSTRtime[3],
		euSTRtime[4],euSTRtime[5],euSTRtime[6],euSTRtime[7],euSTRtime[8],euSTRtime[9]);
	
	fclose(res);
	free(rndNXtime);
	free(euNXtime);
	free(rndSTRtime);
	free(euSTRtime);
}