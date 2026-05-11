#include "rtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int compare(const void* a, const void* b) {
    Entry* p1 = (Entry*)a;
    Entry* p2 = (Entry*)b;

    float c1 = (p1->mbr.x1 + p1->mbr.x2) / (float)2;
    float c2 = (p2->mbr.x1 + p2->mbr.x2) / (float)2;

    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
    return 0;
}

// Calcula el MBR de los pares clave-valor arr, arr es de tamaño size
Rect MBR(Entry arr[], int size) {
    Rect newMBR = {arr[0].mbr.x1, arr[0].mbr.x2, arr[0].mbr.y1, arr[0].mbr.y2};

    for (int i=1; i<size; i++) {
        if (arr[i].mbr.x1 < newMBR.x1) newMBR.x1 = arr[i].mbr.x1;
        if (arr[i].mbr.x2 > newMBR.x2) newMBR.x2 = arr[i].mbr.x2;
        if (arr[i].mbr.y1 < newMBR.y1) newMBR.y1 = arr[i].mbr.y1;
        if (arr[i].mbr.y2 > newMBR.y2) newMBR.y2 = arr[i].mbr.y2;
    }

    return newMBR;
}

// Metodo de bulk-loading Nearest-X
// N el numero de elementos a ocupar del archivo de nombre file
// newFile nombre del archivo que guardara el arbol R-Tree
int nearestX(int N, char *file, char *newFile) {
    Entry *pares = (Entry *)malloc(N*sizeof(Entry));
    FILE *archivo = fopen(file, "rb");
    if (archivo == NULL) {
        perror("Error archivo");
        exit(1);
    }
    float x, y;
    for (int i=0; i<N; i++) {
        fread(&x, sizeof(float), 1, archivo);
        fread(&y, sizeof(float), 1, archivo);
        pares[i].mbr.x1 = x;
        pares[i].mbr.x2 = x;
        pares[i].mbr.y1 = y;
        pares[i].mbr.y2 = y;
        pares[i].child = -1;
    }
    fclose(archivo);
    
    qsort(pares, N, sizeof(Entry), compare);
    Node *arbol = (Node *)malloc( (((N-1)/(B-1)) +3) *sizeof(Node));
    int nodeCount = 0;

    int offset = 1;
    while (N > B) {
        int pares_i = 0;
        int N_temp = N;
        int pares_off = 0;
        for (int i=offset; i<((N/B +1)+offset); i++) {
            arbol[i].k = 0;
            for (int j=0; j<fmin(N_temp, B); j++) {
                arbol[i].children[j] = pares[j+(B*pares_off)];
                arbol[i].k++;
            }
            Rect mbr = MBR(arbol[i].children, arbol[i].k);
            Entry newPar = {mbr, i};
            pares[pares_i] = newPar;
            pares_i++;
            pares_off++;
            nodeCount++;
            N_temp -= B;
        }
        N = N/B +1;
        offset += N;
        qsort(pares, N, sizeof(Entry), compare);
    }

    arbol[0].k = 0;
    for (int j=0; j<N; j++) {
        arbol[0].children[j] = pares[j];
        arbol[0].k++;
    }
    nodeCount++;

    FILE *newArchivo = fopen(newFile, "wb");
    fwrite(arbol, sizeof(Node), nodeCount, newArchivo);
    fclose(newArchivo);

    free(arbol);
    free(pares);
    return nodeCount;
}