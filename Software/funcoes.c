#include "funcoes.h"

/*--------------- FUNÇÕES ----------------*/

void imprimeResultado(int n, Info *info){
	int x;
	FILE *data;

	data = fopen("saida", "w");
	for(x = 0; x < n; x++){
		fprintf(data, "%d\n", info[x].resultado);
	}

	printf("Arquivo de resultados, salvo com sucesso.\n");
}

Info * interpretaEntrada(int *a, char *arq){
	int b, c, d, x, y, cLinha = 0;
	Info *info;
	FILE *data;

	// Abre e lê arquivo de entrada.
	data = fopen(arq, "r");
	if(data == NULL){
		printf("Falha ao abrir o arquivo '%s'!\n", arq);
		return 0;
	}

	// Pega os valores do arquivo e grava na struct.
	while(!feof(data)){
		if(cLinha >= 1){
			for(x = 0; x < *a; x++){
				fscanf(data, "%d %d\n", &b, &c);
				info[x].quantPlanetas = b; // Guarda a quantidade de planetas.
				info[x].quantSaltos = c; // Guarda a quantidade de saltos possíveis.
				info[x].distancias = (int*) malloc((b + 1) * sizeof(int)); // Aloca vetor para guardar os valores de distâncias.
				for(y = 0; y < b + 1; y++){
					fscanf(data, "%d\n", &d);
					info[x].distancias[y] = d; // Guarda valores de cada distancia disponível, para uma configuração.
				}
			}
		}else if(cLinha == 0){
			fscanf(data, "%d\n", a);
			info = (Info*) malloc((*a) * sizeof(Info)); // Aloca vetor para guardar todos as configurações presentes no arquivo.
			cLinha++;
		}
	}

	fclose(data);

	return info;
}

/*----------------------------------------*/

/*-------------- ALGORITMOS --------------*/

/* FORÇA BRUTA */
int fat(int n){
	return (n == 1 || n == 0) ? 1: n * fat(n - 1);
}

void testaCombinacoes(int *menor, int *vetor, int indice, int *data, int inicio, int fim, int r){
	int i, j, soma = 0;

	if (indice == r){
		for (j = 0; j < r; j++) printf("%d ", data[j]);
		for (j = 0; j < r; j++){
			soma += data[j];
		}
		printf("soma:%d\n", soma);

		if(soma <= *menor) *menor = soma;

		return;
	}

	for(i = inicio; i <= fim && fim-i+1 >= r-indice; i++){
		data[indice] = vetor[i];
		testaCombinacoes(menor, vetor, indice + 1, data, i + 1, fim, r);
	}
}

void somaMenorCombinacao(int *menor, int *vetor, int n, int r, int c){
	int *data;

	// Vetor que contém os itens a serem guardados na matriz.
	data = (int*) malloc(r * sizeof(int));

	testaCombinacoes(menor, vetor, 0, data, 0, n - 1, r);

	printf("menor:%d\n\n", *menor);
}

void executaForcaBruta(int n, Info *info){
	int x, q;
	float numComb;


	// Faz todos os testes em força bruta.
	for(x = 0; x < n; x++){
		info[x].menorSoma = 1000000000;

		q = (info[x].quantPlanetas + 1) - info[x].quantSaltos;

		// Determina o número máximo de combinações para uma configuração a ser testada.
		int fatA = fat(info[x].quantPlanetas + 1);
		int fatB = fat(info[x].quantSaltos);
		int fatC = fat(q);
		numComb = fatA / (fatB * fatC);

		somaMenorCombinacao(&info[x].menorSoma, info[x].distancias, info[x].quantPlanetas + 1, q, (int)numComb);

		// A partir da configuração com salto de menor valor, busca agora o maior salto entre as distancias atuais.


		// Grava o resultado na variável de saída.
		// info[x].resultado = menorSoma;

	}
}

/* SOLUÇÃO GULOSA */
void executaAlgGuloso(int n, Info *info){
	int x;

	// Faz todos os testes na solução gulosa.
	for(x = 0; x < n; x++){

	}
}

/* PROGRAMAÇÃO DINÂMICA */
void executaProgDinamica(int n, Info *info){
	int x;

	// Faz todos os testes em programação dinâmica.
	for(x = 0; x < n; x++){

	}
}

/*----------------------------------------*/
