#include "matriz-operacoesvOpenMP.h"

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


	for (int i=0; i < mat_c->lin; i++)
		for (int j=0; j < mat_c->col; j++)
			mat_c->matriz[i][j] = mat_a->matriz[i][j]+mat_b->matriz[i][j];	
	return mat_c;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mymatriz *mmultiplicarvSimples (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
	mymatriz *mat_c = NULL;
	//mymatriz *mat_aux = NULL;
	tipo = tipo % 2;
	printf("Multiplicar\n");
	if ((mat_a->col == mat_b->lin)){
		mat_c = (mymatriz *) malloc (sizeof(mymatriz));
		mat_c->lin = mat_a->lin;
		mat_c->col = mat_b->col;
	}else{
		printf("Matriz a: %d %d \n", mat_a->lin, mat_a->col);
		printf("Matriz b: %d %d \n", mat_b->lin, mat_b->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}

	if (malocar(mat_c)) {	printf ("ERROR: Out of memory\n"); }
	

	for( int i=0; i< mat_c->lin; i++)
		for (int j = 0; j < mat_c->col; j++) {
			mat_c->matriz[i][j] = 0;
			for (int k = 0; k < mat_a->col; k++) {
				mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
			}
		}

	return mat_c;
}

mymatriz *mmultiplicarvBloco (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
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
	mat_c = mmultiplicarBlocos( submatA, submatB, nLinha, nColuna, 0);
	limpaBloco(submatA);
	limpaBloco(submatB);
	return mat_c;
}

mymatriz *mmultiplicarBlocos ( matriz_bloco_t *bloc_a, matriz_bloco_t *bloc_b, int nLinha, int nColuna, int profundidade){
	mymatriz *mat_c = NULL;
	mymatriz *mat_c1 = NULL;
	mymatriz *mat_c2 = NULL;
	matriz_bloco_t *bloco_a1 = NULL;
	matriz_bloco_t *bloco_a2 = NULL;
	matriz_bloco_t *bloco_b1 = NULL;
	matriz_bloco_t *bloco_b2 = NULL;
	int tam_a;

	//printf("bloco a - colunas %i a %i\n", bloc_a->bloco->col_inicio, bloc_a->bloco->col_fim);
	//printf("bloco b - colunas %i a %i\n", bloc_b->bloco->lin_inicio, bloc_b->bloco->lin_fim);
	
	/*printf("Submatrizes:\n\n");
	mostraSubmatriz(bloc_a);
	mostraSubmatriz(bloc_b);*/

	if (	(bloc_a->bloco->col_fim - bloc_a->bloco->col_inicio > 1) 
			&& (bloc_b->bloco->lin_fim - bloc_b->bloco->lin_inicio > 1) 
			&& (profundidade < 4)) {
		if ((bloc_a->bloco->col_fim - bloc_a->bloco->col_inicio) % 2 == 0) {
			tam_a = (bloc_a->bloco->col_fim - bloc_a->bloco->col_inicio) / 2;
		}else{
			tam_a = (bloc_a->bloco->col_fim - bloc_a->bloco->col_inicio +1) / 2;
		}
		bloco_a1 = criaBloco(bloc_a->matriz, bloc_a->bloco->lin_inicio, bloc_a->bloco->lin_fim	, bloc_a->bloco->col_inicio, 		bloc_a->bloco->col_fim -tam_a);
		bloco_a2 = criaBloco(bloc_a->matriz, bloc_a->bloco->lin_inicio, bloc_a->bloco->lin_fim	, bloc_a->bloco->col_fim -tam_a, 	bloc_a->bloco->col_fim);
		//printf("%i\n", bloc_b->bloco->lin_fim -tam_a);
		bloco_b1 = criaBloco(bloc_b->matriz, bloc_b->bloco->lin_inicio, 	bloc_b->bloco->lin_fim -tam_a, 	bloc_b->bloco->col_inicio, bloc_b->bloco->col_fim);
		bloco_b2 = criaBloco(bloc_b->matriz, bloc_b->bloco->lin_fim -tam_a, bloc_b->bloco->lin_fim, 		bloc_b->bloco->col_inicio, bloc_b->bloco->col_fim);
		
		/*mostraSubmatriz(bloco_a1);
		mostraSubmatriz(bloco_a2);
		mostraSubmatriz(bloco_b1);
		mostraSubmatriz(bloco_b2);*/

		mat_c1 = mmultiplicarBlocos( bloco_a1, bloco_b1, nLinha, nColuna, profundidade+1);
		mat_c2 = mmultiplicarBlocos( bloco_a2, bloco_b2, nLinha, nColuna, profundidade+1);
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
		//mgerar(mat_c, 0);
		multiplicar_submatriz(bloc_a, bloc_b, mat_c); 
		//mimprimir( mat_c);
	}
	//printf("\nMultiplica submatrizes\n");
	return mat_c;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mymatriz *mmultiplicarvOpenMP (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
	mymatriz *mat_c = NULL;
	//mymatriz *mat_aux = NULL;
	tipo = tipo % 2;
	printf("Multiplicar\n");
	if ((mat_a->col == mat_b->lin)){
		mat_c = (mymatriz *) malloc (sizeof(mymatriz));
		mat_c->lin = mat_a->lin;
		mat_c->col = mat_b->col;
	}else{
		printf("Matriz a: %d %d \n", mat_a->lin, mat_a->col);
		printf("Matriz b: %d %d \n", mat_b->lin, mat_b->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}

	if (malocar(mat_c)) {	printf ("ERROR: Out of memory\n"); }
	
	#pragma omp parallel for shared(mat_c) 
	for( int i=0; i< mat_c->lin; i++)
		for (int j = 0; j < mat_c->col; j++) {
			mat_c->matriz[i][j] = 0;
			for (int k = 0; k < mat_a->col; k++) {
				mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
			}
		}

	return mat_c;
	/*mymatriz *mat_c = NULL;
	mymatriz **mat_subC = NULL;
	mymatriz *mat_aux = NULL;
	matriz_bloco_t *submatA = NULL;
	matriz_bloco_t *submatB = NULL;
	int nLinha, nColuna;
	int n_threads = omp_get_max_threads();

	if ((mat_a->col != mat_b->lin)){
		printf("Matriz a: %d %d \n", mat_a->lin, mat_a->col);
		printf("Matriz b: %d %d \n", mat_b->lin, mat_b->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}
	//printf("N_Threads: %i\n", n_threads);

	nLinha = mat_a->lin;
	nColuna = mat_b->col;

	mat_subC = (mymatriz **) malloc (sizeof(mymatriz *) * nLinha);
	for( int i = 0; i < nLinha; i++){
		mat_subC[i] = (mymatriz *) malloc (sizeof(mymatriz));
		mat_subC[i]->lin = nLinha;
		mat_subC[i]->col = nColuna;
		if (malocar(mat_subC[i])) {	printf ("ERROR: Out of memory\n"); }
	}
	#pragma omp parallel for num_threads(n_threads) shared ( mat_a, mat_b, mat_subC) private (submatA, submatB)
		for( int i = 0; i < nLinha; i++){
			//printf("Thread %i ira do bloco %i ao %i\n", omp_get_thread_num(), i, i+1);
			submatA = criaBloco(mat_a->matriz, 0	, nLinha, 	i,	i+1);
			submatB = criaBloco(mat_b->matriz, i, i+1, 0, nColuna);
			multiplicar_submatriz(submatA, submatB, mat_subC[i]); 
			//mat_subC[i] = mmultiplicarBlocos( submatA, submatB, nLinha, nColuna, 0);
			limpaBloco(submatA);
			limpaBloco(submatB);
			//mimprimir(mat_subC[i]);
		}
	//printf("Fim For\n");
	//mimprimir(mat_subC[0]);

	// #### SOMAR AS SUBMATRIZES ####
	if (nLinha > 1)
		mat_c = msomar(mat_subC[0], mat_subC[1], 0);
	else
		mat_c = mat_subC[0];
	//printf("Here1\n");
	for (int i = 2; i < nLinha; ++i) {
		//printf("Here 2 %i\n", i);
		//mimprimir(mat_c);
		mat_aux = mat_c;
		mat_c = msomar( mat_c, mat_subC[i], 0);
		mliberar(mat_aux);
		free(mat_aux);
		mliberar(mat_subC[i]);
		free(mat_subC[i]);
		//printf("Limpo %i\n", i);
	}
	free(mat_subC);
	return mat_c;*/

}

/*Sempre A x B = C*/
void multiplicar_submatriz (matriz_bloco_t *bloc_a, matriz_bloco_t *bloc_b, mymatriz *mat_c){
	//int k = bloc_a->bloco->col_fim -1;
	int linhas = mat_c->lin;
	int colunas = mat_c->col;
	int ini = bloc_a->bloco->col_inicio;
	int fim = bloc_a->bloco->col_fim;
	/*printf("Matriz resultante: [%i, %i]\n", linhas, colunas);
	printf("Submatrizes ini: %i fim: %i\n", ini+1, fim);
	mostraSubmatriz(bloc_a);
	mostraSubmatriz(bloc_b);*/
	for (int i = 0; i < linhas; ++i){
		//printf("%i range(0, %i)", i, linhas);
		for (int j = 0; j < colunas; ++j){
			//printf("\t%i range(0, %i)", j, colunas);
			mat_c->matriz[i][j] = 0;
			for ( int k = ini; k < fim; k++){
				//printf("; %i", k);
				mat_c->matriz[i][j] += bloc_a->matriz[i][k] * bloc_b->matriz[k][j];
			}
		}
	}
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
	int truncate = 0;

	if (lin_fim - lin_inicio > 15) {
		truncate = 1;
		lin_fim = lin_inicio+15;
	}
	if (col_fim - col_inicio > 15){
		truncate = 1;
		col_fim = col_inicio+15;
	}
	if (truncate == 1)
		printf("### Submatriz Truncada em 15x15 ###\n");

	printf("Submatriz:\tlinha\tcoluna\n");
	printf("\tinicio: %i\t %i\n", lin_inicio, col_inicio);
	printf("\tfim:\t%i\t %i\n", lin_fim, col_fim);
	for (int i = col_inicio; i < col_fim; ++i)
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