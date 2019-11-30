#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct T_Info{
	int **matDist, quantPlanetas, quantSaltos;
	int resultado;
}Info;

/* funcoes.c */
void imprimeResultado(int n, Info *info);
Info * interpretaEntrada(int *n, char *entrada);
int fat(int n);
void combinacao(int **mat, int *somaDist, int *buffer, int indice, int *k, int inicio, int fim, int r);
void buscaCombinacoes(int **mat, int *somaDist, int n, int r, int c);
int encontraMaior(int **mat, int somaResult, int tam);
int somaBuffer(int *b, int tam);

void executaForcaBruta(int n, Info *info);
void executaAlgGuloso(int n, Info *info);
void executaProgDinamica(int n, Info *info);

/* arg.c */
void getarg(int argc, char **argv, char **alg);
