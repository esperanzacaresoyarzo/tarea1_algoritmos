#include "rtree.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int compareX(const void* a, const void* b) {
    Entry* e1 = (Entry*)a;
    Entry* e2 = (Entry*)b;

    float c1 = centerX(e1->mbr);
    float c2 = centerX(e2->mbr);

    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
    return 0;
}

int compareY(const void* a, const void* b) {
    Entry* e1 = (Entry*)a;
    Entry* e2 = (Entry*)b;

    float c1 = centerY(e1->mbr);
    float c2 = centerY(e2->mbr);

    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
    return 0;
}

int buildSTRLevel(Entry* entries, int n, Node* nodes, int* nodeCount, Entry* parentEntries) {
    qsort(entries, n, sizeof(Entry), compareX);

    int numNodes = (n + B - 1) / B;
    int S = (int)ceil(sqrt((double)numNodes));

    int parentCount = 0;
    int sliceSize = (int)ceil((double)n / S);

    for (int sliceStart = 0; sliceStart < n; sliceStart += sliceSize) {
        int sliceEnd = sliceStart + sliceSize;
        if (sliceEnd > n) sliceEnd = n;

        qsort(entries + sliceStart, sliceEnd - sliceStart, sizeof(Entry), compareY);

        for (int groupStart = sliceStart; groupStart < sliceEnd; groupStart += B) {
            int groupEnd = groupStart + B;
            if (groupEnd > sliceEnd) groupEnd = sliceEnd;

            Node node;
            node.k = groupEnd - groupStart;

            for (int i = 0; i < node.k; i++) {
                node.children[i] = entries[groupStart + i];
            }

            int index = *nodeCount;
            nodes[index] = node;
            (*nodeCount)++;

            parentEntries[parentCount].mbr = computeMBR(entries, groupStart, groupEnd);
            parentEntries[parentCount].child = index;
            parentCount++;

            if (parentCount > n) {
                printf("Error: parentCount excede n\n");
                exit(1);
}
        }
    }

    return parentCount;
}

int buildSTR(Entry* entries, int n, Node* nodes) {
    int nodeCount = 1; // nodes[0] reservado para la raíz

    Entry* currentEntries = malloc(sizeof(Entry) * n);

    if (!currentEntries) {
        return -1;
    }

    for (int i = 0; i < n; i++) {
        currentEntries[i] = entries[i];
    }

    int currentCount = n;

    while (currentCount > B) {
        Entry* parentEntries = malloc(sizeof(Entry) * currentCount);

        if (!parentEntries) {
            free(currentEntries);
            return -1;
        }

        int parentCount = buildSTRLevel(
            currentEntries,
            currentCount,
            nodes,
            &nodeCount,
            parentEntries
        );

        for (int i = 0; i < parentCount; i++) {
            currentEntries[i] = parentEntries[i];
        }

        currentCount = parentCount;

        free(parentEntries);
    }

    Node root;
    root.k = currentCount;

    for (int i = 0; i < currentCount; i++) {
        root.children[i] = currentEntries[i];
    }

    nodes[0] = root;

    free(currentEntries);

    return nodeCount;
}