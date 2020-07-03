#include "matriz-operacoesv3.h"

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% MATRIZ %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
	mymatriz *mat_c = NULL;
	//printf("here soma 1\n");
	if ((mat_a->lin != mat_b-> lin) || (mat_a->col != mat_b->col)){
		printf("Matriz a: %d %d \n", mat_a->lin, mat_a->col);
		printf("Matriz b: %d %d \n", mat_b->lin, mat_b->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}

	mat_c = (mymatriz *) malloc (sizeof(mymatriz));
	mat_c->lin = mat_a->lin;
	mat_c->col = mat_a->col;

	if (malocar(mat_c)) {	printf ("ERROR: Out of memory\n"); }

	if (tipo == 1) {
		for (int i=0; i < mat_c->lin; i++)
		  for (int j=0; j < mat_c->col; j++)
					mat_c->matriz[i][j] = mat_a->matriz[i][j]+mat_b->matriz[i][j];
	} else {
		for (int j=0; j < mat_c->col; j++)
			for (int i=0; i < mat_c->lin; i++)
					mat_c->matriz[i][j] = mat_a->matriz[i][j]+mat_b->matriz[i][j];
	}


  return mat_c;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mymatriz *mmultiplicar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
	mymatriz *mat_c = NULL;
	tipo = tipo % 2;
	matriz_bloco_t *submatA = NULL;
	matriz_bloco_t *submatB = NULL;

	if ((tipo == 0) && (mat_a->col == mat_b->lin)){
		mat_c = (mymatriz *) malloc (sizeof(mymatriz));
		mat_c->lin = mat_a->lin;
		mat_c->col = mat_b->col;

	} else if ((tipo == 1) && (mat_a->lin == mat_b->col)){
		mat_c = (mymatriz *) malloc (sizeof(mymatriz));
		mat_c->lin = mat_b->lin;
		mat_c->col = mat_a->col;

	}else{
		printf("Matriz a: %d %d \n", mat_a->lin, mat_a->col);
		printf("Matriz b: %d %d \n", mat_b->lin, mat_b->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}

	if (malocar(mat_c)) {	printf ("ERROR: Out of memory\n"); }
	mgerar(mat_c, 0);
	
	printf("Gera submatriz\n");
	submatA = (matriz_bloco_t * ) malloc (sizeof( matriz_bloco_t));
	if (newBloco == NULL) printf("ciraSubmatriz ERROR: Out of memory\n");
	criaSubmatriz(mat_a->matriz, &submatA, 0, mat_a->lin, 0, mat_a->col);

	submatB = (matriz_bloco_t * ) malloc (sizeof( matriz_bloco_t));
	if (newBloco == NULL) printf("ciraSubmatriz ERROR: Out of memory\n");
	criaSubmatriz(mat_b->matriz, &submatB, 0, mat_b->lin, 0, mat_b->col);

	submatC = (matriz_bloco_t * ) malloc (sizeof( matriz_bloco_t));
	if (newBloco == NULL) printf("ciraSubmatriz ERROR: Out of memory\n");
	criaSubmatriz(mat_c->matriz, &submatC, 0, mat_c->lin, 0, mat_c->col);

	//printf("printBloco %i\n", submatTeste->bloco->lin_inicio);
	//mostraSubmatriz( &submatTeste);
	for ( i = 0; i < min(mat_a->lin, mat_b->col), i++){
		submatA->bloco->col_inicio = i;
		submatA->bloco->col_fim = i+1;

		submatB->bloco->lin_inicio = i;
		submatB->bloco->lin_fim = i+1;

		multiplicar_submatriz( submatA, submatB, submatC);
	}

	mat_c = submatC->matriz;

	return mat_c;
}

/*Sempre A x B = C*/
int multiplicar_submatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc){
	for ( int ic = mat_subc->bloco->lin_inicio; ic < mat_subc->bloco->lin_fim; ic++)
		for ( int jc = mat_subc->bloco->col_inicio; jc < mat_subc->bloco->col_fim; jc++){
			for(int k = 0; k <  mat_suba->bloco->lin_fim; k++)
				mat_subc->matriz[ic][jc] += mat_suba->matriz[ic][k] * mat_subb->matriz[k][jc];
		}
	return 0;
}

int criaSubmatriz( int **mat, matriz_bloco_t *submat, int lin_inicio, int lin_fim, int col_inicio, int col_fim){
	bloco_t *newBloco = NULL;
	newBloco = (bloco_t *) malloc (sizeof(bloco_t));
	if (newBloco == NULL) printf("ciraSubmatriz ERROR: Out of memory\n");
	newBloco->lin_inicio = lin_inicio;
	newBloco->lin_fim = lin_fim;
	newBloco->col_inicio = col_inicio;
	newBloco->col_fim = col_fim;

	submat->bloco = newBloco;
	submat->matriz = mat;
	return 0;
}

void mostraSubmatriz( matriz_bloco_t *submat){
	printf("mostraSubmatriz 1\n");
	int lin_inicio = submat->bloco->lin_inicio;
	int lin_fim = submat->bloco->lin_fim;
	int col_inicio = submat->bloco->col_inicio;
	int col_fim = submat->bloco->col_fim;

	printf("Submatriz:\tlinha\tcoluna\n");
	printf("\tinicio: %i\t %i\n", lin_inicio, col_inicio);
	printf("\tfim:\t%i\t %i\n", lin_fim, col_fim);
	printf("Valores:");
	for (int i = lin_inicio; i < col_fim; i++){
			printf("\t\n");
			for(int j = col_inicio; j < col_fim; j++)
				printf("%i\t", submat->matriz[i][j]);
	}
	printf("\nFim Submatriz\n");
}

int min(int a, int b){
	if( a > b)
		return b;
	else
		return a;
}