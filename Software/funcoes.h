#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct T_Info{
	int *distancias, quantPlanetas, quantSaltos;
	int **matriz, resultado;
}Info;

/* funcoes.c */
void imprimeResultado(int n, Info *info);
Info * interpretaEntrada(int *n, char *entrada);
int fat(int n);
void combinacao(int **mat, int *k, int *vetor, int indice, int *data, int inicio, int fim, int r);
void buscaCombinacoes(int **mat, int *k, int *vetor, int n, int r, int c);
void obtemDados(int x, Info *info);
int encontraMaior(int menor, Info *info);

int aplicaFB(int x, Info *info);
void executaForcaBruta(int n, Info *info);

int aplicaAG(int x, Info *info);
void executaAlgGuloso(int n, Info *info);

int aplicaPD(int x, Info *info);
void executaProgDinamica(int n, Info *info);

/* arg.c */
void getarg(int argc, char **argv, char **alg);
