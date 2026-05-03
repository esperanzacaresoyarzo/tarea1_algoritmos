#include "rtree.h"


float centerX(Rect r) {
    return (r.x1 + r.x2) / 2.0f;
}

float centerY(Rect r) {
    return (r.y1 + r.y2) / 2.0f;
}

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