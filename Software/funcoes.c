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

void combinacao(int **mat, int *somaDist, int *buffer, int indice, int *k, int inicio, int fim, int r){
	int i, j, soma = 0;

	// Caso base da recursão.
	if (indice == r){
		buffer[indice] = mat[*k-1][*k]; // Adiciona o último valor.

		for (j = 0; j < r + 1; j++) soma += buffer[j];

		if(soma <= *somaDist) *somaDist = soma;

		return;
	}

	// Controla os passos da permutação.
	for(i = inicio; (i < fim) && ((fim-i)+1 > (r-indice)); i++){
		buffer[indice] = mat[i-1][i];
		*k = i + 1;
		combinacao(mat, somaDist, buffer, indice + 1, k, i + 1, fim, r);
	}
}

void buscaCombinacoes(int **mat, int *somaDist, int n, int r, int c){
	int *buffer, k = 0;

	// Vetor que contém os valores a serem somados.
	buffer = (int*) calloc(r + 1, sizeof(int));

	// Inicia a recursão que busca as combinações.
	combinacao(mat, somaDist, buffer, 0, &k, 1, n - 1, r);
}

int encontraMaior(int **mat, int somaResult, int tam){
	int i;

	for(i = 1; i < tam; i++){
		if(somaResult < mat[i-1][i]) somaResult = mat[i-1][i];
	}

	return somaResult;
}

/*----------------------------------------*/

/*-------------- ALGORITMOS --------------*/

/* FORÇA BRUTA */
void executaForcaBruta(int n, Info *info){
	int x, q;
	float numComb;

	// Faz todos os testes em força bruta.
	for(x = 0; x < n; x++){
		info[x].resultado = 2147483647; // Inicializa a variável que receberá a soma da menor distância.

		// Quantidade de planetas que serão removidos.
		q = info[x].quantPlanetas - info[x].quantSaltos;

		// Determina o número máximo de combinações para uma configuração a ser testada.
		int fatA = fat(info[x].quantPlanetas);
		int fatB = fat(info[x].quantSaltos);
		int fatC = fat(q);
		numComb = fatA / (fatB * fatC);

		// Inicia a busca das combinações.
		buscaCombinacoes(info[x].matDist, &info[x].resultado, info[x].quantPlanetas + 2, q, (int)numComb);

		// Após encontrar salto de menor valor, busca agora o maior salto entre as distancias originais.
		info[x].resultado = encontraMaior(info[x].matDist, info[x].resultado, info[x].quantPlanetas + 2); // Grava o resultado na variável de saída.
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
