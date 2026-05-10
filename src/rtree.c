#include "rtree.h"
#include <stdio.h>

float centerX(Rect r) {
    return (r.x1 + r.x2) / 2.0f;
}

float centerY(Rect r) {
    return (r.y1 + r.y2) / 2.0f;
}

// computeMBR: Entry* int int -> Rect
// Recibe un arreglo de entradas, una posicion inicial y una posicion final no inclusiva
// y retorna el MBR que contiene todas las entradas entre start y end - 1
Rect computeMBR(Entry* entries, int start, int end) {
    Rect r = entries[start].mbr;

    for (int i = start + 1; i < end; i++) {
        if (entries[i].mbr.x1 < r.x1) r.x1 = entries[i].mbr.x1;
        if (entries[i].mbr.x2 > r.x2) r.x2 = entries[i].mbr.x2;
        if (entries[i].mbr.y1 < r.y1) r.y1 = entries[i].mbr.y1;
        if (entries[i].mbr.y2 > r.y2) r.y2 = entries[i].mbr.y2;
    }

    return r;
}

// readPoints: const char* Entry* int -> int
// Recibe la ruta de un archivo binario, un arreglo de entradas y la cantidad maxima a leer
// retorna la cantidad de puntos leidos, o -1 si no pudo abrir el archivo

int readPoints(const char* filename, Entry* entries, int maxPoints) {
    FILE* file = fopen(filename, "rb");

    if (!file) {
        return -1;
    }

    int count = 0;

    while (count < maxPoints) {
        float x, y;

        if (fread(&x, sizeof(float), 1, file) != 1) break;
        if (fread(&y, sizeof(float), 1, file) != 1) break;

        entries[count].mbr.x1 = x;
        entries[count].mbr.x2 = x;
        entries[count].mbr.y1 = y;
        entries[count].mbr.y2 = y;
        entries[count].child = -1;

        count++;
    }

    fclose(file);

    return count;
}

// writeTree: const char* Node* int -> void
// Recibe una ruta de salida, un arreglo de nodos y la cantidad de nodos.
// escribe el arbol en disco
void writeTree(const char* filename, Node* nodes, int nodeCount) {
    FILE* file = fopen(filename, "wb");

    if (!file) {
        printf("No se pudo crear el archivo del arbol\n");
        return;
    }

    fwrite(nodes, sizeof(Node), nodeCount, file);
    fclose(file);
}