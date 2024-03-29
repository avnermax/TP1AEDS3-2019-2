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
				if(c < b || c != 0){ // Testa o número de saltos.
					info[x].quantSaltos = c; // Guarda a quantidade de saltos possíveis.
				}else{
					printf("Número de saltos inválido.\n");
					exit(EXIT_SUCCESS);
				}

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
		buffer[indice] = mat[*k-1][*k]; // Adiciona o último valor no buffer.

		for (j = 0; j < r + 1; j++){
			// printf("buffer[%d]:%d\n", j, buffer[j]);
			soma += buffer[j];
		}

		if(soma <= *somaDist) *somaDist = soma;
		// printf("soma:%d\n", soma);
		printf("\n");

		return;
	}

	// Controla os passos da combinação.
	for(i = inicio; i < fim && fim-i+1 >= r-indice; i++){
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

	free(buffer);
}

int encontraMaior(int **mat, int somaResult, int tam){
	int i;

	for(i = 1; i < tam; i++)
		if(somaResult < mat[i-1][i]) somaResult = mat[i-1][i];

	return somaResult;
}

int pegaMenor(int tam, int *buffer, int *vetValores){
	int menor, x, y;

	// Atribui a primeira posição para encontrar o menor;
	for(x = 0; x < tam; x++){
		if(buffer[x] == 0){
			menor = vetValores[0];
			y = x;
		}
	}
	// Procura o menor, entre os valores que não estão presentes no buffer.
	for(x = y; x < tam; x++){
		if(buffer[x] != 0 && vetValores[x] < menor){
			// Salva o menor indice.
			menor = x;
		}
	}

	return menor;
}

void contaPlanetas(int tam, int q, int *buffer, int *vetValores){
	int x, i, conta = 0;

	do{

		for(x = 0; x < tam; x++) if(buffer[x] != 0) conta++;
		if(conta < q){
			i = pegaMenor(tam, buffer, vetValores);
			buffer[i] = vetValores[i];
		}

	}while(conta < q);
}

int somaBuffer(int *buffer, int *vet, int tam){
	int i, soma = 0, aux = 2147483647;

	// 'buffer' controla as posições que devem ser somadas.
	// 'vet' contém os valores que serão somados.

	for(i = tam; i > 0; i--){
		if(buffer[i] != 0){
			if(i == 0){ // Posição igual ao início do buffer.
				soma += vet[i+1] + vet[i];
			}else if(i > 0 && i < tam-1){ // Posição diferente do início e do fim do buffer.
				if(buffer[i] != 0 && buffer[i-1] == 0 && buffer[i+1] == 0){
					if(vet[i-1] + vet[i] < vet[i+1] + vet[i]){
						soma += vet[i-1] + vet[i];
					}else{
						soma += vet[i+1] + vet[i];
					}
				}else{
					if(buffer[i] != 0 && buffer[i-1] != 0 && buffer[i+1] != 0){
						soma += vet[i-1] + vet[i] + vet[i+1];
					}else{
						if(buffer[i] != 0 && buffer[i-1] != 0){
							soma += vet[i-1] + vet[i];
						}else if(buffer[i] != 0 && buffer[i+1] != 0){
							soma += vet[i+1] + vet[i];
						}
					}
				}
				if(aux > soma) aux = soma;
			}
		}else if(buffer[i] == 0){
			soma = 0;
		}
	}

	return aux;
}
/*----------------------------------------*/

/*-------------- ALGORITMOS --------------*/
/* FORÇA BRUTA */
void executaForcaBruta(int n, Info *info){
	int x, q;
	float numComb;

	// Faz todos os testes em força bruta.
	for(x = 0; x < n; x++){
		// Inicializa a variável que receberá a soma da maior distância.
		info[x].resultado = 2147483647;

		// Quantidade de planetas que serão removidos.
		q = info[x].quantPlanetas - info[x].quantSaltos;

		// Determina o número máximo de combinações para uma configuração a ser testada.
		long int fatA = fat(info[x].quantPlanetas);
		long long int fatB = fat(info[x].quantSaltos);
		long int fatC = fat(q);
		numComb = fatA / (fatB * fatC);

		// Inicia a busca das combinações.
		buscaCombinacoes(info[x].matDist, &info[x].resultado, info[x].quantPlanetas + 2, q, (int)numComb);

		// Após encontrar salto de menor valor, busca agora o maior salto entre as distancias originais.
		info[x].resultado = encontraMaior(info[x].matDist, info[x].resultado, info[x].quantPlanetas + 2);
	}
}

/* SOLUÇÃO GULOSA */
void executaAlgGuloso(int n, Info *info){
	int q, x, y, soma, somaResult, *buffer, *vetValores;
	float media;

	// Faz todos os testes na solução gulosa.
	for(x = 0; x < n; x++){
		// Zera variável que guarda soma das distâncias.
		soma = 0;

		// Soma todas as distâncias, para calcular a média.
		for(y = 1; y < info[x].quantPlanetas + 2; y++) soma += info[x].matDist[y-1][y];

		// Calcula a média das distâncias.
		media = soma / (info[x].quantPlanetas + 1);

		// Aloca vetor para armazenar os 'q' valores menores que a media.
		buffer = (int*) calloc(info[x].quantPlanetas + 1, sizeof(int));
		// Aloca vetor para armazenar todas as distâncias.
		vetValores = (int*) calloc(info[x].quantPlanetas + 1, sizeof(int));

		for(y = 1; y < info[x].quantPlanetas + 2; y++){
			// Preenche 'buffer' com valores menores que a média.
			if(info[x].matDist[y-1][y] <= (int)media) buffer[y-1] = info[x].matDist[y-1][y];

			// Preenche 'vetValores' todos valores da matriz para teste posterior.
			vetValores[y-1] = info[x].matDist[y-1][y];
		}

		// Quantidade de planetas que devem conter no buffer.
		q = info[x].quantPlanetas - info[x].quantSaltos;

		// Verifica se os 'n-k' planetas foram preenchidos no buffer.
		contaPlanetas(info[x].quantPlanetas + 1, q, buffer, vetValores);

		// TESTE PARA MOSTRAR PREENCHIMENTO CORRETO DO BUFFER.
		// for(int i=0; i<info[x].quantPlanetas + 1; i++){
		// 	printf("buffer[%d]%d\n", i, buffer[i]);
		// }
		// printf("\n");

		// Soma valores e adjacentes que são menores que a media.
		somaResult = somaBuffer(buffer, vetValores, info[x].quantPlanetas + 1);

		// Após encontrar salto de menor valor, busca agora o maior salto entre as distancias originais.
		info[x].resultado = encontraMaior(info[x].matDist, somaResult, info[x].quantPlanetas + 2);

		// Libera vetores utilizados para teste.
		free(buffer);
		free(vetValores);
	}
}

/* PROGRAMAÇÃO DINÂMICA */
void executaProgDinamica(int n, Info *info){
	int x;

	// Faz todos os testes em programação dinâmica.
	for(x = 0; x < n; x++){

		//

	}
}

/*----------------------------------------*/
