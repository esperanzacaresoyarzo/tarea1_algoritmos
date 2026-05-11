# Tarea 1 - Bulk-loading de R-trees

CC4102 - Diseño y Análisis de Algoritmos

## Integrantes
- Esperanza Cares
- Mario Garcia
- Ricardo Ogno

## Lenguaje
C

## Estructura del proyecto

```text
src/        Código fuente
data/       Datasets de entrada
trees/    Árboles serializados
```
## Compilar

```bash
gcc src/*.c -lm -o main
```
## Uso
Ejecutar los programas

```bash
main
```
Esto ejecuta buildTree y queryExperiment, con el primer programa se construyen los arboles con los dos metodos de bulk-loading para los dataset random.bin y europa.bin
y guarda en un archivo timeResults.txt los tiempos en crear los arboles para distintos N y en trees los archivos binarios de los arboles. El segundo programa consulta los puntos 
contenidos en cuadrados de tamaño s= {0.0025,0.005,0.01,0.025,0.05} y guarda en queryResults.txt la cantidad promedia de lecturas de disco y de puntos encontrados.

## Validacion
Tamano Node: 4096 bytes
