#include "matriz-operacoesvMine.h"

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

	if ( tipo == 1){
		mat_c = mmultiplicar( mat_b, mat_a, 0);
	}else if (!(mat_a->col == mat_b->lin)) {
		printf("Matriz a: %d %d \n", mat_a->lin, mat_a->col);
		printf("Matriz b: %d %d \n", mat_b->lin, mat_b->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}
	
	submatA = criaBloco(mat_a->matriz, 0, mat_a->lin, 0, mat_a->col);
	if (submatA == NULL) {
		printf("MINE > mmultiplicar > erro criação submatriz\n");
		return NULL;
	}
	mostraSubmatriz( submatA);

	submatB = criaBloco(mat_b->matriz, 0, mat_b->lin, 0, mat_b->col);
	if (submatB == NULL) {
		printf("MINE > mmultiplicar > erro criação submatriz\n");
		return NULL;
	}
	mostraSubmatriz( submatB);

	mat_c = (mymatriz *) malloc (sizeof(mymatriz));
	if (malocar(mat_c)) {	printf ("ERROR: Out of memory\n"); }
	mgerar(mat_c, 0);
	mat_c->lin = mat_a->lin;
	mat_c->col = mat_b->col;
	//mat_c = mmultiplicarBloco( submatA, submatB, mat_a->lin, mat_b->col);
	multiplicar_submatriz( submatA, submatB, &mat_c);
	return mat_c;
}

mymatriz *mmultiplicarBloco ( matriz_bloco_t *bloc_a, matriz_bloco_t *bloc_b, int nLinha, int nColuna){
	mymatriz *mat_c = NULL;
	mymatriz *mat1 = NULL;
	mymatriz *mat2 = NULL;
	matriz_bloco_t *submatA1 = NULL;
	matriz_bloco_t *submatA2 = NULL;
	matriz_bloco_t *submatB1 = NULL;
	matriz_bloco_t *submatB2 = NULL;


	if (	(bloc_a->bloco->col_inicio == bloc_a->bloco->col_fim) 
			|| ( bloc_b->bloco->lin_inicio == bloc_b->bloco->lin_fim)) {
		printf("Cria novos subBlocos\n");
		submatA1 = criaBloco(bloc_a->matriz, 0, bloc_a->bloco->lin_fim, bloc_a->bloco->col_inicio, bloc_a->bloco->col_fim -1);
		if (submatA1 == NULL) {
			printf("MINE > mmultiplicarBloco > erro criação submatriz\n");
			return NULL;
		}
		mostraSubmatriz( submatA1);

		submatA2 = criaBloco(bloc_a->matriz, 0, bloc_a->bloco->lin_fim, bloc_a->bloco->col_fim -1, bloc_a->bloco->col_fim);
		if (submatA2 == NULL) {
			printf("MINE > mmultiplicarBloco > erro criação submatriz\n");
			return NULL;
		}
		mostraSubmatriz( submatA2);

		submatB1 = criaBloco(bloc_b->matriz, bloc_b->bloco->lin_inicio, bloc_b->bloco->lin_fim -1, 0, bloc_b->bloco->col_fim);
		if (submatB1 == NULL) {
			printf("MINE > mmultiplicarBloco > erro criação submatriz\n");
			return NULL;
		}
		mostraSubmatriz( submatB1);

		submatB2 = criaBloco(bloc_b->matriz, bloc_b->bloco->lin_fim -1, bloc_b->bloco->lin_fim, 0, bloc_b->bloco->col_fim);
		if (submatB2 == NULL) {
			printf("MINE > mmultiplicarBloco > erro criação submatriz\n");
			return NULL;
		}
		mostraSubmatriz( submatB1);

	} else {
		mat_c = (mymatriz *) malloc (sizeof(mymatriz));
		if (malocar(mat_c)) {	printf ("ERROR: Out of memory\n"); }
		mgerar(mat_c, 0);
		mat_c->lin = nLinha;
		mat_c->col = nColuna;
	}
	return mat_c;
}

/*Sempre A x B = C*/
void multiplicar_submatriz (matriz_bloco_t *bloc_a, matriz_bloco_t *bloc_b, mymatriz *mat_c){
	printf("multiplicar_submatriz\n");
	for (int i = 0; i < mat_c->lin; i++)
		for (int j = 0; j < mat_c->col; j++) {
			for( int k = 0; k < bloc_a->bloco->lin_fim; k++)
				mat_c->matriz[i][j] = bloc_a->matriz[k][j] * bloc_b->matriz[i][k];
		}
}

matriz_bloco_t *criaBloco( int **mat, int lin_inicio, int lin_fim, int col_inicio, int col_fim){
	bloco_t *newBloco = NULL;
	matriz_bloco_t *submat = NULL;

	printf("Gera submatriz\n");
	submat = (matriz_bloco_t * ) malloc (sizeof( matriz_bloco_t));
	if (submat == NULL) {
		printf("MINE >> criaBloco ERROR: Out of memory\n");
		return NULL;
	}

	printf("Define bloco submatriz\n");
	newBloco = (bloco_t *) malloc (sizeof(bloco_t));
	if (newBloco == NULL) {
		printf("MINE >> criaBloco ERROR: Out of memory\n");	
		return NULL;
	}
	
	newBloco->lin_inicio = lin_inicio;
	newBloco->lin_fim = lin_fim;
	newBloco->col_inicio = col_inicio;
	newBloco->col_fim = col_fim;

	submat->bloco = newBloco;
	submat->matriz = mat;
	return submat;
}

void mostraSubmatriz( matriz_bloco_t *submat){
	printf("mostraSubmatriz\n");
	int lin_inicio = submat->bloco->lin_inicio;
	int lin_fim = submat->bloco->lin_fim;
	int col_inicio = submat->bloco->col_inicio;
	int col_fim = submat->bloco->col_fim;

	printf("Submatriz:\tlinha\tcoluna\n");
	printf("\tinicio: %i\t %i\n", lin_inicio, col_inicio);
	printf("\tfim:\t%i\t %i\n", lin_fim, col_fim);
	printf("Valores:");
	for (int i = lin_inicio; i < lin_fim; i++){
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