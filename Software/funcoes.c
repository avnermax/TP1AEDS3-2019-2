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

int fat(int n){
	return (n == 1 || n == 0) ? 1: n * fat(n - 1);
}

void combinacao(int **mat, int *k, int *vetor, int indice, int *data, int inicio, int fim, int r){
	int i, j;

	if (indice == r){
		for (j = 0; j < r; j++){
			mat[*k][j] = data[j];
			printf("%d ", mat[*k][j]);
		}
		printf("\n");

		*k += 1;
		return;
	}

	for(i = inicio; i <= fim && fim-i+1 >= r-indice; i++){
		data[indice] = vetor[i];
		combinacao(mat, k, vetor, indice + 1, data, i + 1, fim, r);
	}
}

void buscaCombinacoes(int **mat, int *k, int *vetor, int n, int r, int c){
	int *data;

	// Vetor que contém os itens a serem guardados na matriz.
	data = (int*) malloc(r * sizeof(int));

	// Inicia a recursão que busca as combinações.
	combinacao(mat, k, vetor, 0, data, 0, n - 1, r);
}

void obtemDados(int x, Info *info){
	int y, q, num;
	float numComb;

	num = 0;
	q = (info[x].quantPlanetas + 1) - info[x].quantSaltos;

	// Determina o número máximo de combinações para uma configuração a ser testada.
	int fatA = fat(info[x].quantPlanetas + 1);
	int fatB = fat(info[x].quantSaltos);
	int fatC = fat(q);
	numComb = fatA / (fatB * fatC);

	// Aloca matriz que armazena todas as combinações de uma configurações.
	info[x].matriz = (int**) malloc((int)numComb * sizeof(int*));
	for(y = 0; y < (int)numComb; y++) info[x].matriz[y] = calloc(q, sizeof(int));

	buscaCombinacoes(info[x].matriz, &num, info[x].distancias, info[x].quantPlanetas + 1, q, (int)numComb);
}

int encontraMaior(int menor, Info *info){
	int x, maior;

	maior = menor;
	for(x = 0; x < info[x].quantPlanetas+1; x++){
		if(menor >= maior) maior = menor;
	}

	return maior;
}

/*----------------------------------------*/

/*-------------- ALGORITMOS --------------*/

/* FORÇA BRUTA */
int aplicaFB(int x, Info *info){
	int menor;

	//

	return menor = 0; // valor 0 usado para evitar bug
}

void executaForcaBruta(int n, Info *info){
	int x, menor;

	// Faz todos os testes em força bruta.
	for(x = 0; x < n; x++){
		obtemDados(x, info);

		// A partir das combinações obtidas, precisamos selecionar a que a soma dê a menor distância correta, usando FB.
		menor = aplicaFB(x, info);

		// Após encontrar salto de menor valor, busca agora o maior salto entre as distancias originais.
		info[x].resultado = encontraMaior(menor, info); // Grava o resultado na variável de saída.
	}
}

/* SOLUÇÃO GULOSA */
int aplicaAG(int x, Info *info){
	int menor;

	//

	return menor = 0; // valor 0 usado para evitar bug
}

void executaAlgGuloso(int n, Info *info){
	int x, menor;

	// Faz todos os testes na solução gulosa.
	for(x = 0; x < n; x++){
		obtemDados(x, info);

		// A partir das combinações obtidas, precisamos selecionar a que a soma dê a menor distância correta, usando AG.
		menor = aplicaAG(x, info);

		// Após encontrar salto de menor valor, busca agora o maior salto entre as distancias originais.
		info[x].resultado = encontraMaior(menor, info); // Grava o resultado na variável de saída.
	}
}

/* PROGRAMAÇÃO DINÂMICA */
int aplicaPD(int x, Info *info){
	int menor;

	//

	return menor = 0; // valor 0 usado para evitar bug
}

void executaProgDinamica(int n, Info *info){
	int x, menor;

	// Faz todos os testes em programação dinâmica.
	for(x = 0; x < n; x++){
		obtemDados(x, info);

		// A partir das combinações obtidas, precisamos selecionar a que a soma dê a menor distância correta, usando PD.
		menor = aplicaPD(x, info);

		// Após encontrar salto de menor valor, busca agora o maior salto entre as distancias originais.
		info[x].resultado = encontraMaior(menor, info); // Grava o resultado na variável de saída.
	}
}

/*----------------------------------------*/
