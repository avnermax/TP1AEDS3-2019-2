#include "funcoes.h"

void getarg(int argc, char **argv, char **alg){

char opcoes;

    while ((opcoes = getopt(argc, argv, "a:")) != -1){
	switch (opcoes){
        case 'a':
            *alg = optarg;
            break;
        default:
            printf("Invalid argument or nonexistent.\n");
        }
    }

}
