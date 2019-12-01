#include "funcoes.h"

int main(int argc, char* argv[]){
	int n, x, y;
	char arq[50], *alg = NULL;
	Info *info;

    // Pega qual algoritmo usar, como argumento.
	getarg(argc, argv, &alg);

	// Pega o nome do arquivo de entrada.
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

	// Libera a matriz.
	for(x = 0; x < n; x++){
		for(y = 0; y < info[x].quantPlanetas + 2; y++){
			free(info[x].matDist[y]);
		}
		free(info[x].matDist);
	}
	// Libera a struct.
	free(info);

	return 0;
}
