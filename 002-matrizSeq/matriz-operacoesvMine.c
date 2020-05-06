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
	matriz_bloco_t *submatTeste = NULL;

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
	
	
	//Primeiro testemos a forma IDIOTA
	if (tipo == 0) {
		for( int i=0; i< mat_c->lin; i++)
			for (int j = 0; j < mat_c->col; j++) {
				mat_c->matriz[i][j] = 0;
				for (int k = 0; k < mat_a->col; k++) {
					mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
				}
			}

	}else{
		for( int i=0; i< mat_c->lin; i++)
			for (int j = 0; j < mat_c->col; j++) {
				mat_c->matriz[i][j] = 0;
				for (int k = 0; k < mat_b->col; k++) {
					mat_c->matriz[i][j] += mat_a->matriz[k][j] * mat_b->matriz[i][k];
				}
			}
	}

	if ( criaSubmatriz(&mat_a, 1, 2, 1, 2, &submatTeste) == 1){
		printf("Erro ao criar bloco\n");
	}else{
		printf("printBloco\n");
		mostraSubmatriz( &submatTeste);
	}

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

int criaSubmatriz( mymatriz *mat, int lin_inicio, int lin_fim, int col_inicio, int col_fim, matriz_bloco_t *submat){
	matriz_bloco_t *newBloco = NULL;
	int size[2] = {lin_fim - col_inicio, col_fim - col_inicio};
	printf("criaBloco 1\n");
	newBloco = (matriz_bloco_t *) malloc (sizeof(matriz_bloco_t));
	newBloco->bloco = (bloco_t *) malloc (sizeof(bloco_t));
	newBloco->bloco->lin_inicio = lin_inicio;
	newBloco->bloco->lin_fim= lin_fim;
	newBloco->bloco->col_inicio = col_inicio;
	newBloco->bloco->col_fim = col_fim;

	newBloco->matriz = (int **) malloc( size[0] * sizeof(int *));
	if (!newBloco->matriz) {
		printf("ERROR: Out of memory\n");
		return 1;
	}
  	for (int i = 0; i < size[0]; i++) {
			newBloco->matriz[i] = (int *) malloc(sizeof(int) * size[1]);
			if (!newBloco->matriz) {
				printf("ERROR: Out of memory\n");
				return 1;
			}
	}
	printf("criaBloco 2\n");
	for (int i = 0; i < size[0]; ++i){
		for (int j = 0; j < size[1]; ++j) {
			//newBloco->matriz[i][j] = mat->matriz[lin_inicio + i][col_inicio + j];
			printf("bloco[%i][%i] = %i\t", i, j, newBloco->matriz[i][j]);
		}
		printf("\n");
	}

	printf("criaBloco 3\n");
	submat = newBloco;

	//Para testesm printaremos o bloco aqui
	return 0;
}

void mostraSubmatriz( matriz_bloco_t *submat){
	printf("printBloco 1\n");
	int lin_inicio = submat->bloco->lin_inicio;
	int lin_fim = submat->bloco->lin_fim;
	int col_inicio = submat->bloco->col_inicio;
	int col_fim = submat->bloco->col_fim;
	int size[2] = {	lin_fim - lin_inicio, col_fim - col_inicio};
	printf("Submatriz:\tlinha\tcoluna\n");
	printf("\tinicio: %i\t %i\n", lin_inicio, col_inicio);
	printf("\tfim:%i\t %i\n", lin_fim, col_fim);
	printf("Valores:");
	for (int i = 0; i < size[0]; ++i){
			printf("\t\n");
			for(int j = 0; j < size[1]; j++)
				printf("%i\t", submat->matriz[i][j]);
	}
	printf("\nFim Submatriz\n");
}
