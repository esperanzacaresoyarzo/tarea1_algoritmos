#ifndef RTREE_H
#define RTREE_H

#include <stdio.h>

#define B 204

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    float x1, x2, y1, y2;
} Rect;

typedef struct {
    Rect mbr;
    int child;
} Entry;

typedef struct {
    int k;
    Entry children[B];
    char pad[12];
} Node;

float centerX(Rect r);
float centerY(Rect r);
Rect computeMBR(Entry* entries, int start, int end);

int buildSTR(Entry* entries, int n, Node* nodes);

int readPoints(const char* filename, Entry* entries, int maxPoints);

void writeTree(const char* filename, Node* nodes, int nodeCount);

// Funciones de las consultas
Node readNode(FILE* file, int index, int* ios);

int intersects(Rect rect1, Rect rect2);

void searchNode(FILE* file, int index, Rect query, int* ios, Point* results, int* resultCount);

int query(const char* filename, Rect queryRect, Point* results, int* ios);

#endif