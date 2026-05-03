#ifndef RTREE_H
#define RTREE_H

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

#endif