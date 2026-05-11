#ifndef RTREE_H
#define RTREE_H

#include <stdio.h>

#define B 204

// Estructura con coordenadas x y 2D
typedef struct {
    float x;
    float y;
} Point;
// Estructura de un rectangulo MBR con limites en x e y
typedef struct {
    float x1, x2, y1, y2;
} Rect;
// Estructura de una entrada del R-tree
//mbr es el rectangulo asociado 
// y child el nodo hilo en el arreglo o -1 si es hoja.
typedef struct {
    Rect mbr;
    int child;
} Entry;
// Estructura de un nodo del R-tree
//k es la cantidad de entradas y children el arreglo fijo de hasta B entradas
// y pad lo que falta para completar los bits solicitados.
typedef struct {
    int k;
    Entry children[B];
    char pad[12];
} Node;
//Retorna la coordenada x del centro del rectangulo
float centerX(Rect r);
//Retorna coordenada y del centro del rectangulo
float centerY(Rect r);
// Retorna el calculo del MBR que contiene las entradas entre start
// y end -1
Rect computeMBR(Entry* entries, int start, int end);

//Construye un R-trre usando Sort-tile-recursive
//y retorna la cantidad total de nodos creados
int buildSTR(Entry* entries, int n, Node* nodes);
//Lee puntos desde archivo binario y los convierte en entradas del R-tree
// y retorna la cantidad de puntos leidos, o -1 su no pudo abrir el archivo
int readPoints(const char* filename, Entry* entries, int maxPoints);
//Escribe secuencialmente los nodos del R-tree en un archivo binario
void writeTree(const char* filename, Node* nodes, int nodeCount);

// Funciones de las consultas
Node readNode(FILE* file, int index, int* ios);

int intersects(Rect rect1, Rect rect2);

void searchNode(FILE* file, int index, Rect query, int* ios, Point* results, int* resultCount);

int query(const char* filename, Rect queryRect, Point* results, int* ios);

int nearestX(int N, char *file, char *newFile);

void buildTree();

void queryExperiment();

#endif