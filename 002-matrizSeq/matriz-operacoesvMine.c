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

	if (tipo == 1) {
		for (int i=0; i < mat_c->lin; i++)
			for (int j=0; j < mat_c->col; j++)
				mat_c->matriz[i][j] = mat_a->matriz[i][j]+mat_b->matriz[i][j];
	} else {
		for (int j=0; j < mat_c->col; j++)
			for (int i=0; i < mat_c->lin; i++)
				mat_c->matriz[i][j] = mat_a->matriz[i][j]+mat_b->matriz[i][j];
	}
	//printf("Soma matrizes\n");
	return mat_c;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mymatriz *mmultiplicar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
	mymatriz *mat_c = NULL;
	tipo = tipo % 2;
	matriz_bloco_t *submatA = NULL;
	matriz_bloco_t *submatB = NULL;
	int nLinha, nColuna;

	if ((tipo == 1) && (mat_a->lin != mat_b->col)){
		printf("Matriz a: %d %d \n", mat_b->lin, mat_b->col);
		printf("Matriz b: %d %d \n", mat_a->lin, mat_a->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}else if ((tipo == 0) && (mat_a->col != mat_b->lin)){
		printf("Matriz a: %d %d \n", mat_a->lin, mat_a->col);
		printf("Matriz b: %d %d \n", mat_b->lin, mat_b->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}

	if (tipo == 0) {
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
	}else{
		submatA = criaBloco(mat_b->matriz, 0, mat_b->lin, 0, mat_b->col);
		if (submatA == NULL) {
			printf("MINE > mmultiplicar > erro criação submatriz\n");
			return NULL;
		}

		submatB = criaBloco(mat_a->matriz, 0, mat_a->lin, 0, mat_a->col);
		if (submatB == NULL) {
			printf("MINE > mmultiplicar > erro criação submatriz\n");
			return NULL;
		}
		nLinha = mat_b->lin;
		nColuna = mat_a->col;
	}

	mat_c = mmultiplicarBloco( submatA, submatB, nLinha, nColuna);
	limpaBloco(submatA);
	limpaBloco(submatB);
	return mat_c;
}

void mmultiplicarBloco_worker( void *args){
	param_t1 *p = (param_t1 *) args;
	p->mat_c = mmultiplicarBloco( p->bloco_a, p->bloco_b, p->nLinha, p->nColuna);
	//mimprimir(p->mat_c);
}

mymatriz *mmultiplicarBloco ( matriz_bloco_t *bloc_a, matriz_bloco_t *bloc_b, int nLinha, int nColuna){
	param_t1 *args;
	mymatriz *mat_c = NULL;

	/*
	printf("Submatrizes:\n");
	mostraSubmatriz(bloc_a);
	mostraSubmatriz(bloc_b);
	*/
	if (	(bloc_a->bloco->col_fim - bloc_a->bloco->col_inicio > 1) 
			&& (bloc_b->bloco->lin_fim - bloc_b->bloco->lin_inicio > 1)){
		args = (param_t1 *) malloc (2 * sizeof(param_t1));
		for (int i = 0; i < 2; ++i){
			if ( i == 0) {
				args[i].bloco_a = criaBloco(bloc_a->matriz, bloc_a->bloco->lin_inicio, bloc_a->bloco->lin_fim	, bloc_a->bloco->col_inicio, bloc_a->bloco->col_fim -1);
				args[i].bloco_b = criaBloco(bloc_b->matriz, bloc_b->bloco->lin_inicio, bloc_b->bloco->lin_fim -1, bloc_b->bloco->col_inicio, bloc_b->bloco->col_fim);
			}else {
				args[i].bloco_a = criaBloco(bloc_a->matriz, bloc_a->bloco->lin_inicio, bloc_a->bloco->lin_fim	, bloc_a->bloco->col_fim -1, bloc_a->bloco->col_fim);
				args[i].bloco_b = criaBloco(bloc_b->matriz, bloc_b->bloco->lin_fim -1, bloc_b->bloco->lin_fim	, bloc_b->bloco->col_inicio, bloc_b->bloco->col_fim);
			}
			args[i].nLinha = nLinha;
			args[i].nColuna = nColuna;
			args[i].mat_c = NULL;
		}
		for (int i = 0; i < 2; ++i) {
			mmultiplicarBloco_worker( (void *) (args+i));
		}
		mat_c = msomar(args[0].mat_c, args[1].mat_c, 0);
		for (int i = 0; i < 2; ++i) {
			mliberar(args[i].mat_c);
			free(args[i].mat_c);
			limpaBloco(args[i].bloco_a);
			limpaBloco(args[i].bloco_b);
		}
		free(args);
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