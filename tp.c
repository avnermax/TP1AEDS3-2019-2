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

	for(x = 0; x < n; x++){
		if(strcmp(alg, "FB") == 0){
			executaForcaBruta(info);
		}else if(strcmp(alg, "AG") == 0){
			executaAlgGuloso(info);
		}else if(strcmp(alg, "PD") == 0){
			executaProgDinamica(info);
		}else{
			printf("Algoritmo para resolução digitado incorretamente.\n");
			break;
		}
	}

	return 0;
}
