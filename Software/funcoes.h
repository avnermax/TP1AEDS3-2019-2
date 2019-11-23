#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct T_Info{
	int *distancias, quantPlanetas, quantSaltos;
	int menorSoma, resultado;
}Info;

/* funcoes.c */
void imprimeResultado(int n, Info *info);
Info * interpretaEntrada(int *n, char *entrada);

int fat(int n);
void somaMenorCombinacao(int *menor, int *vetor, int indice, int *data, int inicio, int fim, int r);
int testaCombinacoes(int *menor, int *vetor, int n, int r, int c);

void executaForcaBruta(int n, Info *info);
void executaAlgGuloso(int n, Info *info);
void executaProgDinamica(int n, Info *info);

/* arg.c */
void getarg(int argc, char **argv, char **alg);
