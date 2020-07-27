#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesvThread.h"

#define N_EXEC 2

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	// DECLARAÇÃO de VARIÁVEIS
	mymatriz mat_a, mat_b;
	mymatriz **mat_multT, **mat_multS, **mat_multB;
	char filename[100];
	FILE *fmat;
	int nr_line;
	int *vet_line = NULL;
	int N, M, La, Lb;
	double start_time, end_time;
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
	//printf("Iniciando\n");
	if (argc != 3){
		printf ("ERRO: Numero de parametros %s <matriz_a> <matriz_b>\n", argv[0]);
		exit (1);
	}

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                Leitura da Matriz A (arquivo)
	fmat = fopen(argv[1],"r");
	if (fmat == NULL) {
		printf("Error: Na abertura dos arquivos.");
		exit(1);
	}
	extrai_parametros_matriz(fmat, &N, &La, &vet_line, &nr_line);
	//return 1;
	mat_a.matriz = NULL;
	mat_a.lin = N;
	mat_a.col = La;
	if (malocar(&mat_a)) {
		printf ("ERROR: Out of memory\n");
	}
	filein_matriz (mat_a.matriz, N, La, fmat, vet_line, nr_line);
	free (vet_line);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
	
	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Leitura da Matriz B (arquivo)
	fmat = fopen(argv[2],"r");
	if (fmat == NULL) {
		printf("Error: Na abertura dos arquivos.");
		exit(1);
	}
	extrai_parametros_matriz(fmat, &Lb, &M, &vet_line, &nr_line);
	mat_b.matriz = NULL;
	mat_b.lin = Lb;
	mat_b.col = M;
	if (malocar(&mat_b)) {
		printf ("ERROR: Out of memory\n");
	}
	filein_matriz (mat_b.matriz, Lb, M, fmat, vet_line, nr_line);
	free (vet_line);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
	mimprimir(&mat_a);
	mimprimir(&mat_b);

	printf("\n%%%%%%%%%%%%%%%%\n");
	//mimprimir(&mat_a);
	//mimprimir(&mat_b);
/*
	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//	Operações de Multiplicação
	mat_multS = (mymatriz **) malloc (sizeof(mymatriz *)*N_EXEC);
	for (int ii=0; ii < N_EXEC; ii++) {
		printf("\n ##### multiplicar_t%d de Matrizes Simples #####\n", ii);
		start_time = wtime();
		mat_multS[ii] = mmultiplicarvSimples(&mat_a, &mat_b, ii);
		end_time = wtime();
		//mimprimir(mat_multS[ii]);
		printf("\tRuntime: %f\n", end_time - start_time);
		sprintf(filename, "mult_tSimples%d.result", ii);
		fmat = fopen(filename,"w");
		fileout_matriz(mat_multS[ii], fmat);
		fclose(fmat);
	}
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
*/
	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//	Operações de Multiplicação ultilizando multiplicação em blocos
	mat_multB = (mymatriz **) malloc (sizeof(mymatriz *)*N_EXEC);
	for (int ii=0; ii < N_EXEC; ii++) {
		printf("\n ##### multiplicar_t%d de Matrizes Em Bloco#####\n", ii);
		start_time = wtime();
		mat_multB[ii] = mmultiplicarvBloco(&mat_a, &mat_b, ii);
		end_time = wtime();
		//mimprimir(mat_multB[ii]);
		printf("\tRuntime: %f\n", end_time - start_time);
		//sprintf(filename, "mult_tBlocos%d.result", ii);
		//fmat = fopen(filename,"w");
		//fileout_matriz(mat_multB[ii], fmat);
		//fclose(fmat);
	}
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//	Operações de Multiplicação utilizando Threads
	mat_multT = (mymatriz **) malloc (sizeof(mymatriz *)*N_EXEC);
	for (int ii=0; ii < N_EXEC; ii++) {
		printf("\n ##### multiplicar_t%d de Matrizes Em Threads#####\n", ii);
		start_time = wtime();
		mat_multT[ii] = mmultiplicarvThread(&mat_a, &mat_b, ii);
		end_time = wtime();
		//mimprimir(mat_multT[ii]);
		printf("\tRuntime: %f\n", end_time - start_time);
		//sprintf(filename, "mult_tThreads%d.result", ii);
		//fmat = fopen(filename,"w");
		//fileout_matriz(mat_multT[ii], fmat);
		//fclose(fmat);
	}
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//              Comparação dos resultados
/*
	printf("\n ##### Comparação dos resultados da Multiplicação de matrizes Simples#####\n");
	for (int i=1; i<N_EXEC; i++) {
		printf("[mult_t0 vs mult_t%d]\t", i);
		mcomparar (mat_multS[0],mat_multS[i]);
	}
*/
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//              Comparação dos resultados

	printf("\n ##### Comparação dos resultados da Multiplicação de matrizes por Bloco#####\n");
	for (int i=1; i<N_EXEC; i++) {
		printf("[mult_t0 vs mult_t%d]\t", i);
		mcomparar (mat_multB[0],mat_multB[i]);
	}
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//              Comparação dos resultados

	printf("\n ##### Comparação dos resultados da Multiplicação de matrizes por Thread#####\n");
	for (int i=1; i<N_EXEC; i++) {
		printf("[mult_t0 vs mult_t%d]\t", i);
		mcomparar (mat_multT[0],mat_multT[i]);
	}
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//              Comparação dos resultados
	printf("\n ##### Comparação dos resultados da Multiplicação de matrizes entre implementações #####\n");
/*
	printf("[Simples vs Bloco]\t");
	mcomparar (mat_multS[0],mat_multB[0]);

	printf("[Simples vs Threads]\t");
	mcomparar (mat_multS[0],mat_multT[0]);
*/	
	printf("[Blocos vs Threads]\t");
	mcomparar (mat_multB[0],mat_multT[0]);

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                   Liberação de memória
	/*
	for (int ii=0; ii < N_EXEC; ii++) {
		mliberar(mat_multS[ii]);
		free (mat_multS[ii]);
	}
*/
	for (int ii=0; ii < N_EXEC; ii++) {
		mliberar(mat_multB[ii]);
		free (mat_multB[ii]);
	}

	for (int ii=0; ii < N_EXEC; ii++) {
		mliberar(mat_multT[ii]);
		free (mat_multT[ii]);
	}

	mliberar(&mat_a);
	mliberar(&mat_b);
	free(mat_multT);
	free(mat_multS);
	free(mat_multB);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	return 0;
}
