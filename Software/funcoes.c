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

				// Aloca matriz para guardar os valores de distâncias.
				info[x].matDist = (int**) malloc((b + 2) * sizeof(int*));
				for(y = 0; y < b + 2; y++) info[x].matDist[y] = (int*) calloc((b + 2), sizeof(int));

				for(y = 1; y < b + 2; y++){
					fscanf(data, "%d\n", &d);
					info[x].matDist[y - 1][y] = d; // Guarda valores de cada distancia disponível, para uma configuração.
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

int fat(int n){
	return (n == 1 || n == 0) ? 1: n * fat(n - 1);
}

void combinacao(int **mat, int *somaDist, int indice, int inicio, int fim, int r){
	int i, soma = 0;

	// Caso base da recursão.
	if (indice == r){
		if(soma <= *somaDist) *somaDist = soma;

		return;
	}

	// Controla os passos da permutação.
	for(i = inicio; (i < fim) && ((fim-i)+1 > (r-indice)); i++){
		soma = mat[i-1][i] + mat[i][i+1];
		printf("soma:%d\n", soma);
		combinacao(mat, somaDist, indice + 1, i + 1, fim, r);
	}
}

/*----------------------------------------*/

/*-------------- ALGORITMOS --------------*/

/* FORÇA BRUTA */
void executaForcaBruta(int n, Info *info){
	int x, q;
	float numComb;

	// Faz todos os testes em força bruta.
	for(x = 0; x < n; x++){
		q = info[x].quantPlanetas - info[x].quantSaltos;

		// Determina o número máximo de combinações para uma configuração a ser testada.
		int fatA = fat(info[x].quantPlanetas);
		int fatB = fat(info[x].quantSaltos);
		int fatC = fat(q);
		numComb = fatA / (fatB * fatC);
		printf("numComb:%d\n", (int)numComb);

		// Inicia a recursão que busca as combinações.
		combinacao(info[x].matDist, &info[x].resultado, 0, 1, info[x].quantPlanetas+1, (int)numComb);

		for(int i=0; i<info[x].quantPlanetas+2; i++){
			for(int j=0; j<info[x].quantPlanetas+2; j++){
				if(info[x].matDist[i][j] != 0){
					printf("\033[1;31m%d ", info[x].matDist[i][j]);
				}else{
					printf("\033[0m%d ", info[x].matDist[i][j]);
				}
			}
			printf("\n");
		}
		printf("\n");

		// A partir das combinações obtidas, precisamos selecionar a que a soma dê a menor distância correta, usando FB.
		// menor = aplicaFB(x, info);

		// Após encontrar salto de menor valor, busca agora o maior salto entre as distancias originais.
		// info[x].resultado = encontraMaior(menor, info); // Grava o resultado na variável de saída.
	}
}

/* SOLUÇÃO GULOSA */
void executaAlgGuloso(int n, Info *info){
	int x;

	// Faz todos os testes na solução gulosa.
	for(x = 0; x < n; x++){

		// A partir das combinações obtidas, precisamos selecionar a que a soma dê a menor distância correta, usando AG.
		// menor = aplicaAG(x, info);

		// Após encontrar salto de menor valor, busca agora o maior salto entre as distancias originais.
		// info[x].resultado = encontraMaior(menor, info); // Grava o resultado na variável de saída.
	}
}

/* PROGRAMAÇÃO DINÂMICA */
void executaProgDinamica(int n, Info *info){
	int x;

	// Faz todos os testes em programação dinâmica.
	for(x = 0; x < n; x++){

		// A partir das combinações obtidas, precisamos selecionar a que a soma dê a menor distância correta, usando PD.
		// menor = aplicaPD(x, info);

		// Após encontrar salto de menor valor, busca agora o maior salto entre as distancias originais.
		// info[x].resultado = encontraMaior(menor, info); // Grava o resultado na variável de saída.
	}
}

/*----------------------------------------*/
