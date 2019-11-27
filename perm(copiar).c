#include<stdio.h>
#include<stdlib.h>

int verifica_adj(int vetor[], int num, int n, int r, int salva){
	int i, j;

	for(i=0; i < n; i++){
		if(vetor[i] == salva){
			j=i;
			break;
		}
	}

	for(i=0; i < n; i++){
		if((vetor[j+1] == vetor[num]) || (vetor[j-1] == vetor[num])){
			return 1;
		}else{
			return 0;
		}
	}
}


int verifica_indice(int *num, int r){
	int i, j;
	for(i=0; i<r; i++){
		for(j=0; j!=i && j<r; j++){
			if(num[i] == num[j]){
				return 0;
			}
		}
	}
	return 1;
}


int vetor[] = {1,8,1,7, 1, 1, 1, 1, 2};
//int vetor[] = {1,2,3,1};
 
int main() {
	//int *vetor[] = {1,2,3,1};
    /* vetor que representara cada permutacao. */
    int *num;
    /* quantidade de elementos do vetor. */
    int n;
    /* tamanho de cada permutacao. */
    int r=4;
    /* controle de loop. */
    int i, j ;
    
 	int salva=0;
 	int count=0;
 	//salva = (int *)malloc(6 * sizeof(int)) ;

    /* Obtem a quantidade de elementos do vetor. */
   n=9;
    //while ( vetor[n] != NULL ) n++ ;
 
    /* Testa parametros da linha de comando. */
   
    /* Aloca espaco para o vetor num. Lembre-se
     * que o vetor `num' representa um numero
     * na base n com r algarismos. */
    num = (int *)malloc((r+1) * sizeof(int)) ;
    //num[0]=0;
 	//num[1]=0;
   	int j_aux;
    /* Inicio do algoritmo. */
    while (num[r] == 0 ) {
        for(i=0; i < n; i++){
            /* Mostra a permutacao na tela. */
            for(j=0; j < r; j++) {
            	if(count == 0){
	            	if(verifica_indice(num, r) == 1){
	                	printf("%d ", vetor[num[j]]);
	                	//if(verifica_adj(vetor, num, n, r, salva) == 1){
	                		salva = salva+(vetor[num[j]]);
	                		//printf("SALVA = %d\n", salva);
	                		count++;
	                }
                }else{
                	j_aux=num[j];
                	if(verifica_indice(num, r) == 1){
	                	printf("%d ", vetor[num[j]]);
	                	if(verifica_adj(vetor, j_aux, n, r, salva) == 1){
	                		salva = salva+(vetor[num[j]]);
	                		printf("SALVA = %d\n", salva);
	                		
	                	//	count = 0;
                		}else{
                			salva=0;
                		//	count=0;
                		}
                	}
                }
                
                //printf("SALVA = %d\n", salva);
            }
            count = 0;
            //printf("\n");
            //printf("SALVA = %d\n", salva);
            //num[1]=1;
            salva=0;
 
            /* incrementa o algarismo menos
             * significativo. */
            num[0]++ ;
        }
 		

        /* distribui os vai-uns. */
        for(i=0; i < r; i++) {
            if(num[i] == n) {
                num[i] = 0;
                num[i+1]++ ;
            }
        }
    	//salva=0;
    }
 
    free(num) ;
 
    return 0;
}