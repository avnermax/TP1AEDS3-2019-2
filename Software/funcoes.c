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
void executaForcaBruta(int n, Info *info){
int x, y, q, soma, *vetIndSoma;

	// Faz todos os testes em força bruta.
	for(x = 0; x < n; x++){
		q = ((info[x].quantPlanetas + 1) - info[x].quantSaltos) + 1;
		vetIndSoma = (int*) malloc(q * sizeof(int));
		while(1){
			for(y = 0; y < q; y++){
				vetIndSoma[y] = info[x].distancias[y];
			}
			soma = somaIndices(vetIndSoma, info[x].distancias, q);

			// percorrer nas posições restantes.

		}
	}
}

int somaIndices(int *i, int *vet, int q){
int x;

	for(x = 0; x < q; x++){
		soma = soma + vet[i[x]];
	}

	return soma;
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
