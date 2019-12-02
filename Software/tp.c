#include "funcoes.h"


int main(int argc, char* argv[]){
	int n, x, y;
	char *arq, *alg = NULL;
	Info *info;

	struct rusage resources;
	struct timeval u_inicio, u_fim, s_inicio, s_fim;

    // Pega qual algoritmo usar, como argumento.
	getarg(argc, argv, &alg);

	// Aloca variável apra guardar o nome do arquivo, recebido pelo usuário.
	arq = (char*) malloc(50 * sizeof(char));

	// Pega o nome do arquivo de entrada.
	printf("Digite o nome do arquivo de entrada\n");
	printf(":: ");
	scanf("%s", arq);

	// Inicia a contagem do tempo de execução.
    gettimeofday(&u_inicio, NULL);
    getrusage(RUSAGE_SELF, &resources);
    s_inicio = resources.ru_utime;
	clock_t inicio = clock();

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

	// Finaliza a contagem do tempo de execução.
    gettimeofday(&u_fim, NULL);
	getrusage(RUSAGE_SELF, &resources);
	s_fim = resources.ru_utime;
	clock_t fim = clock();

	// Calcula o tempo de execução.
    printf("\nSystem Runtime: %.8f seconds\n", (double)(s_fim.tv_sec - s_inicio.tv_sec) / 1000000);
    printf("User Runtime: %.8f seconds\n", (double)(s_fim.tv_usec - s_inicio.tv_usec) / 1000000);
	printf("Time: %.8f seconds\n", (double)(fim - inicio) / CLOCKS_PER_SEC);

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
