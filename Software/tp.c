// teste de instalação do git no PC Óricles
#include "funcoes.h"
int main(int argc, char* argv[]){
int n;
char arq[30], *alg = NULL;
Info *info;

    // Pega qual algoritmo usar, como argumento.
	getarg(argc, argv, &alg);

	// Pega o nome do arquivo de entrada.
	printf("Digite o nome do arquivo de entrada\n");
	printf(":: ");
	scanf("%s", arq);

	printf("teste");

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
