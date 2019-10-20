#include "funcoes.h"

int main(int argc, char* argv[]){
int x, n;
char arq[30], *alg = NULL;
Info *info;

    // Pega os arquivos passados como argumento.
	getarg(argc, argv, &alg);

	// Digite o nome do arquivo de entrada.
	printf("Digite o nome do arquivo de entrada\n");
	printf(":: ");
	scanf("%s", arq);

	info = interpretaEntrada(&n, arq);

	if(strcmp(alg, "FB") == 0){
		executaForcaBruta(n, info);
	}else if(strcmp(alg, "AG") == 0){
		executaAlgGuloso(n, info);
	}else if(strcmp(alg, "PD") == 0){
		executaProgDinamica(n, info);
	}else{
		printf("Falha ao escolher o algoritmo de resolução do problema.\n");
		return 0;
	}

	imprimeResultado(n, info); // Guarda resultados no arquivo 'saida'.

	return 0;
}
