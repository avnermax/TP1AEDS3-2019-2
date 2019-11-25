#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>



void combinationUtil(int *a,int **arr, int *aux, int inic, int fim, int index,int c, int r,int *k,int s,int h){
  int i, j,maior=0;
  printf("\tinic:%d fim:%d index:%d c:%d r:%d s:%d h:%d \n", i+1, fim, index+1,c, r,s,h);
  if (index == r && inic<fim+1){
    aux[index-1] = arr[c][inic];
    printf("\tR  a[%d]=M[%d][%d] = %d \n",index-1,c,inic, aux[index-1] );
    if(inic == fim){
      for(int g=0;g<index;g++)
        if(maior<aux[g])
          maior = aux[g];
      if(maior<k[0])
        k[0]= maior;

    }
    return;
  }
  //printf("index != r\n" );
  for (i=inic; i<=fim && fim-i+1>=r-index; i++){
      //printf("idx: %d\n",index );

      aux[index] = arr[index][i];
      //printf(" h: %d\n",h );
      if(index+1 == r && h==0){
        c=i-1;
        h++;
      }
      //printf(" h: %d\n",h );
      printf("\t>  a[%d]=M[%d][%d]: %d\n",index,index,i,arr[index][i]);
      //if(a[0]<r){
      printf("\ti:%d fim:%d index:%d c:%d r:%d s:%d h:%d\n ", i, fim, index,c, r,s,h);
        combinationUtil(a,arr, aux, i+1, fim, index+1,c, r,k,s,h);
    //  }
      //else{
    //  combinationUtil(a,arr, aux, i+1, fim, i+1,c, r,k,s,h);
      //}


  }
  printf("\t" );
  for (j=0; j<r; j++)
    printf("%d ", aux[j]);
  printf("\nK:%d r:%d\n",k[0],r);
  printf("-------------\n" );
  h=0;
}

int main()
{
  int r = 3,**M,soma,f,n = 7;;
  M = (int**)calloc(6,sizeof(int*));
  for(int i=0;i<n-1;i++)
    M[i] = (int*)calloc(7,sizeof(int));
  int v[]={3,6,4,2,1,2};
  for (int w = 0; w < 6; w++) {
    soma=0;
    f=w;
    for (int y = w+1; y < 7; y++) {
      soma=soma+v[f++];
      M[w][y]=soma;
    }
  }

  printf(" %3d %3d %3d %3d %3d %3d %3d\n",0,1,2,3,4,5,6 );
  printf("__________________________\n");
  for ( int w1 = 0 ; w1 < 6 ; w1++ ) {

    for ( int y1 = 0  ; y1 < 7 ; y1++ ) {
      printf(" %3d",M[w1][y1]);
    }
    printf("\n" );
  }
  int aux[r],a[1],k[1];
  a[0]=r;
  k[0]=100000000;
  combinationUtil(a,M, aux, 1, n-1, 0, 0, r,k ,0,0);
}


/*
void comb(int *arr, size_t n, size_t index) {
    //printf("****** %d\n",n );
    size_t k;
    if (index == n) {
        // array vazio, imprime o que está "para trás"
        //printf("%d\n", arr[0]);
        for (k = 0; k < n; k++)
        printf(" %d", arr[k]);
        puts("");
        return;
    }

    for (k = index; k < n; k++) {
        int tmp;
        // mete cada um dos elementos ao principio
        tmp = arr[k];
        arr[k] = arr[index];
        arr[index] = tmp;

        // recursividade!
        //printf(">>%d\n", index);
        comb(arr, n, index + 1);

        // repoe posicao inicial
        tmp = arr[k];
        arr[k] = arr[index];
        arr[index] = tmp;
    }
}

int main(void) {
    int v[] = {1, 2, 3, 4};

    for(int i =3;i>0;i--){
      printf(">>>> %d >>>>>\n",i );
      comb(v, sizeof v / sizeof *v, i);
    }


    return 0;
}


int main(void) {
    int soma, **M;


    comb(M, 6 ,0 );
    return 0;
}


//SO FUNCIONA O PRIMEIRO LOOP


void combinationUtil(int a,int **arr, int *aux, int inic, int fim, int index, int r,int c,int h, int k){
  int i, j;
  //printf("index: %d\n",index);
  if (index == a){
    aux[index-1] = arr[inic-1][c];
    printf("  a[%d]=M[%d][%d] = %d \n",index-1,inic-1,c, aux[index-1] );
    return;
  }
  //printf("index != r\n" );
  for (i=inic; i<=fim && fim-i+1 >= r-index; i++){
      aux[index] = arr[index][i];

      if(index+1 == r && h==0){
        printf("a[%d]=M[%d][%d]: %d\n",index,index,i,arr[index][i]);
        r--;
        c=i;
        h++;
      }
      if(index+1<r && k==0){
        //printf("a[%d]=M[%d][%d]: %d\n",index-1,arr[index-1][i-1],aux[index-1]);
        printf("a[%d]=M[%d][%d]: %d\n",index,index,i,arr[index][i]);
        aux[index] = arr[i][i ];
        index=i;
        k++;
      }
      combinationUtil(a,arr, aux, i+1, fim, index+1, r,c,h,k);
      if(r<a)r++;
  }
  printf("\t" );
  for (j=0; j<r; j++)
    printf("%d ", aux[j]);
  printf("\n");
  printf("-------------\n" );
    h=0 ;k=0;
}

int main()
{
  int r = 3,**M,soma,n = 6;;
  M = (int**)calloc(6,sizeof(int*));
  for(int i=0;i<6;i++)
    M[i] = (int*)calloc(6,sizeof(int));
  int v[]={3,6,4,2,1,2};
  for (int w = 0; w 	< 6; w++) {
    soma=0;
    for (int y = w; y < 6; y++) {
      soma=soma+v[y];
      M[w][y]=soma;
      M[y][w]=soma;
    }
  }

  printf(" %3d %3d %3d %3d %3d %3d\n",0,1,2,3,4,5 );
  printf("__________________________\n");
  for ( int w1 = 0 ; w1 < 6 ; w1++ ) {

    for ( int y1 = 0  ; y1 < 6 ; y1++ ) {
      printf(" %3d",M[w1][y1]);
    }
    printf("\n" );
  }
  int aux[r];
  combinationUtil(r,M, aux, 0, n-1, 0, r,0,0,0);
}




*/
