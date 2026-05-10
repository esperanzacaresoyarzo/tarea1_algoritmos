#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"

int main() {
    
    printf("Tamano Node: %lu bytes\n", sizeof(Node));

    int maxPoints = 1000000;

    Entry* entries = malloc(sizeof(Entry) * maxPoints);
    int maxNodes = maxPoints / B + 1000;
    Node* nodes = malloc(sizeof(Node) * maxNodes);

    if (!entries || !nodes) {
        printf("Error de memoria\n");
        return 1;
    }

    int n = readPoints("data/europa.bin", entries, maxPoints);

    if (n < 0) {
        printf("No se pudo leer dataset\n");
        free(entries);
        free(nodes);
        return 1;
    }

    int totalNodes = buildSTR(entries, n, nodes);
    writeTree("results/str_europa.bin", nodes, totalNodes);

    printf("STR construido correctamente\n");
    printf("Puntos leidos: %d\n", n);
    printf("Total nodos: %d\n", totalNodes);
    printf("Hijos raiz: %d\n", nodes[0].k);

    free(entries);
    free(nodes);

    return 0;
}