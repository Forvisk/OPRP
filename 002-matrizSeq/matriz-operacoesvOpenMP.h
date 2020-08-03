#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <omp.h>
#include "matrizv3.h"

#ifndef SOME_HEADER_FILE_H
#define SOME_HEADER_FILE_H
typedef struct {
  int lin_inicio;
  int lin_fim;
  int col_inicio;
  int col_fim;
} bloco_t;

typedef struct {
  int **matriz;
  bloco_t *bloco;
} matriz_bloco_t;

typedef struct {
  int **matriz;
  int lin;
  int col;
} mymatriz;

#endif

mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo);
mymatriz *mmultiplicarvSimples (mymatriz *mat_a, mymatriz *mat_b, int tipo);
mymatriz *mmultiplicarvBloco (mymatriz *mat_a, mymatriz *mat_b, int tipo);
mymatriz *mmultiplicarvOpenMP (mymatriz *mat_a, mymatriz *mat_b, int tipo);

mymatriz *mmultiplicarBlocos (matriz_bloco_t *bloc_a, matriz_bloco_t *bloc_b, int nLinha, int nColuna, int profundidade);

void multiplicar_submatriz (matriz_bloco_t *bloc_a, matriz_bloco_t *bloc_b, mymatriz *mat_c);

matriz_bloco_t *criaBloco( int **mat, int lin_inicio, int lin_fim, int col_inicio, int col_fim);

void mostraSubmatriz( matriz_bloco_t *submat);
int min( int a, int b);
int limpaBloco( matriz_bloco_t *sbloco);