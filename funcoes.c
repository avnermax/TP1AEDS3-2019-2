#include "funcoes.h"

/*--------------- FUNÇÕES ----------------*/
FILE * opData(char *arq){
	FILE *data = fopen(arq, "r");
	if(data == NULL){
		printf("Falha ao abrir o arquivo %s!\n", arq);
		exit(EXIT_FAILURE);
		return 0;
	}
	return data;
}

Info * interpretaEntrada(int *a, char *arq){
int b, c, d, x, y;
Info *info;
FILE *data;

    // Abre arquivo de entrada.
	data = opData(arq);

	// Pega os valores do arquivo e grava na struct.
	while(!feof(data)){
		fscanf(data, "%d\n", a);
		info = (Info*) malloc((*a) * sizeof(Info));
		for(x = 0; x < *a; x++){
			fscanf(data, "%d %d\n", &b, &c);
			info[x].quantPlanetas = b;
			info[x].quantSaltos = c;
			info[x].distancias = (int*) malloc((b + 1) * sizeof(int)); // Aloca vetor para guardar os valores de distâncias.
			for(y = 0; y < b + 1; b++){
				fscanf(data, "%d\n", &d);
				info[x].distancias[y] = d;
			}
		}
	}

    return info;
}

/*----------------------------------------*/

/*-------------- ALGORITMOS --------------*/

/* FORÇA BRUTA */
void executaForcaBruta(Info *info){

}

/* SOLUÇÃO GULOSA */
void executaAlgGuloso(Info *info){

}

/* PROGRAMAÇÃO DINÂMICA */
void executaProgDinamica(Info *info){

}

/*----------------------------------------*/
