#include "matriz-operacoesv3.h"

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% MATRIZ %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
	mymatriz *mat_c = NULL;

	if ((mat_a->lin != mat_b-> lin) || (mat_a->col != mat_b->col)){
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
	int nMe = 0;
	/*
	if ((mat_a->lin != mat_b->col) && (mat_a->col != mat_b->lin)){
		
	}
*/
	mimprimir(mat_a);
	mimprimir(mat_b);
	
	if ((tipo == 1) && (mat_a->col == mat_b->lin)){
		mat_c = (mymatriz *) malloc (sizeof(mymatriz));
		mat_c->lin = mat_a->lin;
		mat_c->col = mat_b->col;
		nMe = mat_b->lin;
	} else if ((tipo != 1) && (mat_a->lin == mat_b->col)){
		mat_c = (mymatriz *) malloc (sizeof(mymatriz));
		mat_c->lin = mat_b->lin;
		mat_c->col = mat_a->col;
		nMe = mat_a->lin;
	}else{
		printf("Matriz a: %d %d \n", mat_a->lin, mat_a->col);
		printf("Matriz b: %d %d \n", mat_b->lin, mat_b->col);
		printf ("Erro: Matrizes incompatíveis!\n");
		exit(1);
	}

	if (malocar(mat_c)) {	printf ("ERROR: Out of memory\n"); }

	//Primeiro testemos a forma IDIOTA
	if (tipo == 1) {
		for( int i=0; i< mat_c->lin; i++)
			for (int j = 0; j < mat_c->col; j++) {
				mat_c->matriz[i][j] = 0;
				for (int k = 0; k < nMe; k++) {
					mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
				}
			}

	}else{
		for( int i=0; i< mat_c->lin; i++)
			for (int j = 0; j < mat_c->col; j++) {
				mat_c->matriz[i][j] = 0;
				for (int k = 0; k < nMe; k++) {
					mat_c->matriz[i][j] += mat_a->matriz[k][j] * mat_b->matriz[i][k];
				}
			}
	}

	return mat_c;
}