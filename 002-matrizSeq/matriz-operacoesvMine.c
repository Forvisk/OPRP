#include "matriz-operacoesvMine.h"

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% MATRIZ %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
	mymatriz *mat_c = NULL;
	if ((mat_a->lin != mat_b-> lin) || (mat_a->col != mat_b->col)){
		printf("Matriz a: %d %d \n", mat_a->lin, mat_a->col);
		printf("Matriz b: %d %d \n", mat_b->lin, mat_b->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}

	mat_c = (mymatriz *) malloc (sizeof(mymatriz));
	mat_c->lin = mat_a->lin;
	mat_c->col = mat_a->col;

	if (malocar(mat_c))
		printf ("ERROR: Out of memory\n");

	//if (tipo == 1) {
		for (int i=0; i < mat_c->lin; i++)
			for (int j=0; j < mat_c->col; j++)
				mat_c->matriz[i][j] = mat_a->matriz[i][j]+mat_b->matriz[i][j];
	/*} else {
		for (int j=0; j < mat_c->col; j++)
			for (int i=0; i < mat_c->lin; i++)
				mat_c->matriz[i][j] = mat_a->matriz[i][j]+mat_b->matriz[i][j];
	}*/
	//printf("Soma matrizes\n");
	return mat_c;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mymatriz *mmultiplicar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
	mymatriz *mat_c = NULL;
	matriz_bloco_t *submatA = NULL;
	matriz_bloco_t *submatB = NULL;
	int nLinha, nColuna;
	
	if ((mat_a->col != mat_b->lin)){
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

	submatB = criaBloco(mat_b->matriz, 0, mat_b->lin, 0, mat_b->col);
	if (submatB == NULL) {
		printf("MINE > mmultiplicar > erro criação submatriz\n");
		return NULL;
	}
	nLinha = mat_a->lin;
	nColuna = mat_b->col;

	mat_c = mmultiplicarBloco( submatA, submatB, nLinha, nColuna);
	limpaBloco(submatA);
	limpaBloco(submatB);
	return mat_c;
}

mymatriz *mmultiplicarBloco ( matriz_bloco_t *bloc_a, matriz_bloco_t *bloc_b, int nLinha, int nColuna){
	mymatriz *mat_c = NULL;
	mymatriz *mat_c1 = NULL;
	mymatriz *mat_c2 = NULL;
	matriz_bloco_t *bloco_a1 = NULL;
	matriz_bloco_t *bloco_a2 = NULL;
	matriz_bloco_t *bloco_b1 = NULL;
	matriz_bloco_t *bloco_b2 = NULL;

	/*
	printf("Submatrizes:\n");
	mostraSubmatriz(bloc_a);
	mostraSubmatriz(bloc_b);
	*/
	if (	(bloc_a->bloco->col_fim - bloc_a->bloco->col_inicio > 1) 
			&& (bloc_b->bloco->lin_fim - bloc_b->bloco->lin_inicio > 1)){
		
		bloco_a1 = criaBloco(bloc_a->matriz, bloc_a->bloco->lin_inicio, bloc_a->bloco->lin_fim	, bloc_a->bloco->col_inicio, bloc_a->bloco->col_fim -1);
		bloco_b1 = criaBloco(bloc_b->matriz, bloc_b->bloco->lin_inicio, bloc_b->bloco->lin_fim -1, bloc_b->bloco->col_inicio, bloc_b->bloco->col_fim);
		bloco_a2 = criaBloco(bloc_a->matriz, bloc_a->bloco->lin_inicio, bloc_a->bloco->lin_fim	, bloc_a->bloco->col_fim -1, bloc_a->bloco->col_fim);
		bloco_b2 = criaBloco(bloc_b->matriz, bloc_b->bloco->lin_fim -1, bloc_b->bloco->lin_fim	, bloc_b->bloco->col_inicio, bloc_b->bloco->col_fim);
			
		mat_c1 = mmultiplicarBloco( bloco_a1, bloco_a1, nLinha, nColuna);
		mat_c2 = mmultiplicarBloco( bloco_a2, bloco_b2, nLinha, nColuna);
		mat_c = msomar(mat_c1, mat_c2, 0);
		
		mliberar(mat_c1);
		free(mat_c1);
		limpaBloco(bloco_a1);
		limpaBloco(bloco_a2);
		mliberar(mat_c2);
		free(mat_c2);
		limpaBloco(bloco_b1);
		limpaBloco(bloco_b2);

	} else {
		mat_c = (mymatriz *) malloc (sizeof(mymatriz));
		mat_c->lin = nLinha;
		mat_c->col = nColuna;
		if (malocar(mat_c)) {	printf ("ERROR: Out of memory\n"); }
		mgerar(mat_c, 0);
		multiplicar_submatriz(bloc_a, bloc_b, mat_c);
		//mimprimir( mat_c);
	}
	return mat_c;
}

/*Sempre A x B = C*/
void multiplicar_submatriz (matriz_bloco_t *bloc_a, matriz_bloco_t *bloc_b, mymatriz *mat_c){
	int k = bloc_a->bloco->col_fim -1;
	for (int i = 0; i < bloc_a->bloco->lin_fim; ++i)
		for (int j = 0; j < bloc_b->bloco->col_fim; ++j)
			mat_c->matriz[i][j] = bloc_a->matriz[i][k] * bloc_b->matriz[k][j];
}

matriz_bloco_t *criaBloco( int **mat, int lin_inicio, int lin_fim, int col_inicio, int col_fim){
	bloco_t *newBloco = NULL;
	matriz_bloco_t *submat = NULL;

	submat = (matriz_bloco_t * ) malloc (sizeof( matriz_bloco_t));
	if (submat == NULL) {
		printf("MINE >> criaBloco ERROR: Out of memory\n");
		return NULL;
	}

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
	int lin_inicio = submat->bloco->lin_inicio;
	int lin_fim = submat->bloco->lin_fim;
	int col_inicio = submat->bloco->col_inicio;
	int col_fim = submat->bloco->col_fim;

	printf("Submatriz:\tlinha\tcoluna\n");
	printf("\tinicio: %i\t %i\n", lin_inicio, col_inicio);
	printf("\tfim:\t%i\t %i\n", lin_fim, col_fim);
	for (int i = 0; i < col_fim; ++i)
		printf("\t(%i)", i);
	for (int i = lin_inicio; i < lin_fim; i++){
		printf("\n(%i)", i);
		for(int j = col_inicio; j < col_fim; j++)
			printf("\t%i", submat->matriz[i][j]);
	}
	printf("\nFim Submatriz\n");
}

int min(int a, int b){
	if( a > b)
		return b;
	else
		return a;
}

int limpaBloco( matriz_bloco_t *sbloco){
	free(sbloco->bloco);
	free(sbloco);
	return 0;
}