#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct T_Info{
	int *distancias, quantPlanetas, quantSaltos;
}Info;

/* funcoes.c */
FILE * opData(char *arq);
Info * interpretaEntrada(int *n, char *entrada);
void executaForcaBruta(Info *info);
void executaAlgGuloso(Info *info);
void executaProgDinamica(Info *info);

/* arg.c */
void getarg(int argc, char **argv, char **alg);
