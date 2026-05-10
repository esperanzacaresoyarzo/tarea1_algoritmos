#include "rtree.h"
#include <stdio.h>

/*
 Lee el nodo en la posicion index del archivo binario del arbol.
 Incrementa *ios en 1 por cada llamada.
 file: archivo binario abierto en modo "rb"
 index: posicion del nodo en el archivo
 ios: contador de lecturas a disco
 retorna: el nodo leido
 */
Node readNode(FILE* file, int index, int* ios) {
    Node node;
    fseek(file, index * sizeof(Node), SEEK_SET);
    fread(&node, sizeof(Node), 1, file);
    (*ios)++;
    return node;
}

/*
 Determina si dos rectangulos se intersectan.
 rect1, rect2: rectangulos a comparar
 retorna: 1 si se intersectan, 0 si no
 */
int intersects(Rect rect1, Rect rect2) {
    if (rect1.x2 < rect2.x1) return 0;
    if (rect1.x1 > rect2.x2) return 0;
    if (rect1.y2 < rect2.y1) return 0;
    if (rect1.y1 > rect2.y2) return 0;
    return 1;
}

/*
 Busca recursivamente los puntos contenidos en query a partir del nodo en index.
 file: archivo binario del arbol abierto en modo "rb"
 index: indice del nodo actual
 query: rectangulo de consulta
 ios: contador de lecturas a disco
 results: arreglo donde se acumulan los puntos encontrados
 resultCount: cantidad de puntos encontrados hasta ahora
 */
void searchNode(FILE* file, int index, Rect query, int* ios, Point* results, int* resultCount) {
    Node node = readNode(file, index, ios);

    // revisar cada hijo del nodo
    for (int i = 0; i < node.k; i++) {
        Entry e = node.children[i];

        // ignorar hijos cuyo MBR no intersecta con la query
        if (!intersects(e.mbr, query)) continue;

        // si es hoja, agregar el punto a results
        if (e.child == -1) {
            results[*resultCount].x = e.mbr.x1;
            results[*resultCount].y = e.mbr.y1;
            (*resultCount)++;
        } else {
            // si es nodo interno, continuar recursivamente
            searchNode(file, e.child, query, ios, results, resultCount);
        }
    }
}

/*
 Realiza una consulta por rectangulo sobre el arbol guardado en filename.
 filename: ruta del archivo binario del arbol
 queryRect: rectangulo de consulta {x1, x2, y1, y2}
 results: arreglo pre-allocado donde se guardan los puntos encontrados
 ios: se inicializa en 0 y se incrementa por cada lectura a disco
 retorna: cantidad de puntos encontrados
 */
int query(const char* filename, Rect queryRect, Point* results, int* ios) {
    FILE* file = fopen(filename, "rb");
    if (!file) return -1;

    *ios = 0;
    int resultCount = 0;

    searchNode(file, 0, queryRect, ios, results, &resultCount);

    fclose(file);
    return resultCount;
}