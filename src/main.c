#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"

int main() {
    // Ejecuta experimento de construccion de arboles
    printf("Construyendo los árboles del experimento...\n");
    buildTree();
    printf("Contrucción de árboles finalizada. Datos guardados en timeResults.txt\n");

    // Ejecuta experimento de consultas
    printf("Realizando consultas del experimento...\n");
    queryExperiment();
    printf("Consultas realizadas. Datos guardados en queryResults.txt\n");
    return 0;
}
