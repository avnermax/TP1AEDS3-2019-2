#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>


void compara(int*aux,int r,int maior,int *s){
    for(int g=0;g<r;g++)
       if(maior < aux[g]){
          //recebe o maior valor da combinação
          maior = aux[g];
       }
       if(maior<s[0]){
               /*caso o maior valor obtido seja menor 
                que o já analisado*/
                s[0] = maior;
           }
}

void combinationUtil(int *a,int **arr, int *aux, int inic, int fim, int index,int c, int r,int k,int *s){
    int fl,i, j,maior=0;
     
    if (k == r || inic==fim){
      /* se k é igual a quantidade de saltos
      ou inicio é igual a sair do ultimo planeta*/ 
        if(k<r){
           //ké menor que a quantidade de saltos
           aux[r-1] = arr[inic][index];
           compara(aux,r,&maior,s);
           }
           
    }else{
         //se k é igual a quantidade de saltos 
         aux[r-1] = arr[index+1][inic];
         if(index+1==fim){
            //não existe mais nenhuma possibilidades de saltos.
            compara(aux,r,&maior,s);
            }
        }
    return;
        
    }
  
    for (i=inic; i<fim && fim-i+1>=r-index; i++){
       
        if(k < r-1){
            //k é menor que quantidade de planetas a se pousar
            aux[k] = arr[index][i];
            combinationUtil(a, arr, aux, i+1, fim, i, i, r, k+1, s);
            
        }else if(k == r-1){
            //k é igual a quantidade de planetas a pousar
            aux[k-1] = arr[index][i];
            combinationUtil( a, arr, aux, c, fim, i, c, r, k+1, s);
        }else if(k > r-1){
            //k é igual a quantidade de saltos
            aux[k-1] = arr[index+1][i];
            combinationUtil( a, arr, aux, i+1, fim, index, c, r, k, s);
        }
    }
}


int main(){
  /*********Construção de situação exemplo.*************/
  int r = 5,**M,soma,f,n =10,aux[r],a[1],k=0,s[1];
  M = (int**)calloc(n,sizeof(int*));
  for(int i=0;i<n;i++)
    M[i] = (int*)calloc(n,sizeof(int));
  //vetor exemplo
  int v[]={1,8,1,7,1,1,1,1,2};
  // contrução da matriz exemplo
  for (int w = 0; w < n; w++) {
    soma=0;
    f=w;
    for (int y = w+1; y < n; y++) {
      soma=soma+v[f++];
      M[w][y]=soma;
      M[y][w]=soma;
    }
  }
  // impressão da matriz exemplo
  printf("    " );
  for(int g = 0; g< n ; g++ )
  	printf(" %3d",g);
  printf("\n    ____________________________\n");
  for ( int w1 = 0 ; w1 < n ; w1++ ) {
  	printf("%3d|",w1);
    for ( int y1 = 0  ; y1 < n ; y1++ ) {
      printf(" %3d",M[w1][y1]);
    }
    printf("\n");
  }
  printf("\n");
  a[0]=r;
  s[0]=99999;
  combinationUtil(a,M, aux, 1, n-1, 0, 0, r+1,k ,s);
  //Solução
  printf("\n\n\n\t\t%d",s[0]);
}
