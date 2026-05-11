#include "rtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_QUERIES 100
#define NUM_SIZES 5

/*
Calcula la desviacion estandar de un arreglo de valores values, con
n elementos y con promedio mean
*/
static double stddev(double* values, int n, double mean) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = values[i] - mean;
        sum += diff * diff;
    }
    return sqrt(sum / n);
}

/*
Realiza el experimento de consultas sobre los 4 arboles con N=2^24.
Para cada s en {0.0025, 0.005, 0.01, 0.025, 0.05}, genera 100 cuadrados
aleatorios de lado s y consulta cada arbol, registrando I/Os y puntos encontrados.
Guarda los resultados en queryResults.txt.
*/
void queryExperiment() {
    double sizes[NUM_SIZES] = {0.0025, 0.005, 0.01, 0.025, 0.05};
    const char* treeFiles[4] = {
        "trees/randomNX9.bin",
        "trees/europaNX9.bin",
        "trees/randomSTR9.bin",
        "trees/europaSTR9.bin"
    };
    const char* treeNames[4] = {
        "Random Nearest-X",
        "Europa Nearest-X",
        "Random STR",
        "Europa STR"
    };

    int N = 1 << 24; // N = 2^24
    Point* results = malloc(sizeof(Point) * N);
    FILE* out = fopen("queryResults.txt", "w");

    // Semilla para poder repetir experimento.
    srand(66);

    for (int t = 0; t < 4; t++) {
        fprintf(out, "Arbol: %s\n", treeNames[t]);

        for (int si = 0; si < NUM_SIZES; si++) {
            double s = sizes[si];

            double ios_vals[NUM_QUERIES];
            double pts_vals[NUM_QUERIES];

            // Generar 100 consultas de lado s y ejecutarlas
            for (int q = 0; q < NUM_QUERIES; q++) {
                // generar esquina inferior izquierda aleatoria en [0, 1-s]
                float x = ((float)rand() / RAND_MAX) * (1.0f - (float)s);
                float y = ((float)rand() / RAND_MAX) * (1.0f - (float)s);

                Rect queryRect = {x, x + (float)s, y, y + (float)s};

                int ios = 0;
                int found = query(treeFiles[t], queryRect, results, &ios);

                ios_vals[q] = ios;
                pts_vals[q] = found;
            }

            // calcular promedios y desviaciones estandar
            double ios_sum = 0.0, pts_sum = 0.0;
            for (int q = 0; q < NUM_QUERIES; q++) {
                ios_sum += ios_vals[q];
                pts_sum += pts_vals[q];
            }
            double ios_mean = ios_sum / NUM_QUERIES;
            double pts_mean = pts_sum / NUM_QUERIES;
            double ios_std = stddev(ios_vals, NUM_QUERIES, ios_mean);
            double pts_std = stddev(pts_vals, NUM_QUERIES, pts_mean);

            fprintf(out, "s=%.4f: ios_avg=%.2f ios_std=%.2f pts_avg=%.2f pts_std=%.2f\n",
                s, ios_mean, ios_std, pts_mean, pts_std);
        }

        fprintf(out, "\n");
    }

    fclose(out);
    free(results);
}
