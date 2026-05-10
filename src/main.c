#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"

int main() {
    int N = 1 << 15; // 2^15

    Entry* entries = malloc(sizeof(Entry) * N);
    int maxNodes = N / B * 2 + 1000;
    Node* nodes = malloc(sizeof(Node) * maxNodes);
    Point* results = malloc(sizeof(Point) * N);

    if (!entries || !nodes || !results) {
        printf("Error de memoria\n");
        return 1;
    }

    // construir arbol STR con los primeros N puntos del dataset aleatorio
    int n = readPoints("data/random.bin", entries, N);
    if (n < 0) {
        printf("No se pudo leer dataset\n");
        free(entries); free(nodes); free(results);
        return 1;
    }

    int totalNodes = buildSTR(entries, n, nodes);
    writeTree("data/str_random.bin", nodes, totalNodes);

    printf("STR construido\n");
    printf("Puntos leidos: %d\n", n);
    printf("Total nodos: %d\n", totalNodes);
    printf("Hijos raiz: %d\n", nodes[0].k);

    // consulta de prueba: rectangulo centrado en (0.5, 0.5) con lado 0.1
    Rect queryRect = {0.45f, 0.55f, 0.45f, 0.55f};
    int ios = 0;
    int found = query("data/str_random.bin", queryRect, results, &ios);

    printf("\nConsulta: x=[0.45, 0.55] y=[0.45, 0.55]\n");
    printf("Puntos encontrados: %d\n", found);
    printf("Lecturas a disco: %d\n", ios);

    free(entries);
    free(nodes);
    free(results);

    return 0;
}
