#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
//#include "busca_profundidade.h"
// Number of vertices in the graph
#define V 12
#define MAX 5000
#define maxsize_itinerario 5

char cidades[V] = {'A','B','C','D','L','E', 'M', 'F', 'G','H','I','J'};

int restricoes_5P[] = {2,4,8,9};

struct itinerario
{
    int custo; ///custo do itinerário calculado pela heurística do vizinho mais próximo
    int indice;
    int *cidades_iti; ///lista de cidades
    int total_P;
    int num_cidades; ///numero de cidades que fazem parte do itinerario
    int invalido;
    int escolhido;
};

struct itinerario *lista_itinerarios_3P;
struct itinerario *lista_itinerarios_5P;
struct itinerario *lista_itinerarios_10P;

struct solucao
{
    struct itinerario* lista_itinerarios; ///até 10 itinerários diferentes para a frota de caminhões
    int custo_total;
    int num_caminhoes;
    int custo_final; ///soma de custo_total + num_caminhoes
};

void calculaCustoItinerario(int itinerario[], int graph[V][V], int source, int dist[V][V],
                            int shortestPath[V][V][V], int size_itinerario,
                            struct itinerario* lista_itinerarios, int indice_iti);


int fatorial(int n)
{
    if (n>0)
        return n*fatorial(n-1);
    else
        return 1;
}

int permutacao(int n)
{
    return fatorial(n);
}

int arranjo(int n, int p)
{
    if (p>1)
        return n*arranjo(n-1,p-1);
    else
        return n;
}

int combinacao(int n, int p)
{
    return arranjo(n,p)/fatorial(p);
}

int verificaVjNoCaminhoVi(int vj, int vi, int shortestPath[V][V])
{
    for(int j=0; shortestPath[vi][j] != -1; j++)
    {
        if(vj == shortestPath[vi][j])
            return 1;
    }
    return 0;
}

int verificaSeElementoEstaNoVetor(int vetor[], int elemento, int size)
{
    for(int i=0; i < size; i++)
        if(vetor[i] == elemento)
            return 1;
    return 0;
}

int verificaIntersecaoEntreDoisVetoresOrdenados(int arr1[], int arr2[], int m, int n)
{
    int i = 0, j = 0;
    quick_int(arr1, 0, m-1);
    quick_int(arr2, 0, n-1);
    while (i < m && j < n)
    {
        if(arr1[i] != -1 && arr2[j] != -1) //&& arr1[i] != INT_MAX && arr2[j] != INT_MAX
        {
            if (arr1[i] < arr2[j])
                i++;
            else if (arr2[j] < arr1[i])
                j++;
            else /* if arr1[i] == arr2[j] */
            {
                return 1;
            }
        }
        else if (arr1[i] == -1) i++;
        if (arr2[j] == -1) j++;
    }
    return 0;
}

///buscaItinerario(int ind) - percorre a lista de itinerarios
struct itinerario buscaItinerario(int ind, struct itinerario* lista, int size_lista)
{
    struct itinerario vazio;
    for(int i=0; i < size_lista; i++)
        if(lista[i].indice == ind)
            return lista[i];
    return vazio;
};

int buscaProxItinerarioValido(int pos, struct itinerario* lista, int size_lista)
{
    for(int i=pos; i < size_lista; i++)
        if(lista[i].invalido == 0 && lista[i].indice >= 0)
            return lista[i].indice;
    return -1;
};

int buscaPosProxItinerarioValido(int pos, struct itinerario* lista, int size_lista)
{
    for(int i=pos; i < size_lista; i++)
        if(lista[i].invalido == 0 && lista[i].indice >= 0)
            return i;
    return -1;
};

int verificaSeJaExisteConj(int **conjCidades, int conjCandidato[], int m, int n)
{
    int i, j, cont;
    for(i=0; i < m; i++)
    {
        cont = 0;
        for(j=0; j < n; j++)
        {
            if(conjCidades[i][j] == conjCandidato[j])
                cont++;
        }
        if (cont == n)
            return 1;
    }
    return 0;
}

///retorna a quantidade de itinerários depois de filtrar
int filtraItinerarios(struct itinerario* lista_itinerarios, int nomesCidades[],
                      int m, int n)
{
    int i, new_num = 0;
    for(i=0; i < m; i++)
    {
        if (verificaIntersecaoEntreDoisVetoresOrdenados(lista_itinerarios[i].cidades_iti, nomesCidades,
                lista_itinerarios[i].num_cidades, n) == 0)
        {
            printf("Invalidando %d\n", lista_itinerarios[i].indice);
            lista_itinerarios[i].indice = -1; ///invalidando itinerario
        }
        else new_num++;
    }
    return new_num;
}

void invalidaItinerarios(struct itinerario* lista_itinerarios, int nomesCidades[], int m, int n, int pos_escolhido)
{
    int i;
    for(i=0; i < m; i++)
    {
        if(i != pos_escolhido && lista_itinerarios[i].indice != -1)
        {
            if (verificaIntersecaoEntreDoisVetoresOrdenados(lista_itinerarios[i].cidades_iti, nomesCidades,
                    lista_itinerarios[i].num_cidades, n) == 1)
            {
                lista_itinerarios[i].invalido = 1; ///invalidando itinerario
            }
        }
    }
}

void validaItinerarios(struct itinerario* lista_itinerarios, int nomesCidades[],
                       int m, int n)
{
    int i;
    for(i=0; i < m; i++)
    {
        if (verificaIntersecaoEntreDoisVetoresOrdenados(lista_itinerarios[i].cidades_iti, nomesCidades,
                lista_itinerarios[i].num_cidades, n) == 1)
        {
            if (lista_itinerarios[i].indice != -1)
                lista_itinerarios[i].invalido = 0; ///validando itinerario
        }
    }
}

void resetInvalidos(struct itinerario* lista, int size_lista)
{
    for(int i=0; i < size_lista; i++)
        if(lista[i].indice != -1)
            lista[i].invalido = 0;
}

//int verificaIntersecaoComItinerarios(struct itinerario* ) usando memcmp

void avaliaItinerarios(int cidade, int pos, int conjCandidato[], int totaldeP, int *indiceConj,
                       struct itinerario* lista_itinerarios, int necessidades[], int soma,
                       int graph[V][V], int source, int dist[V][V], int shortestPath[V][V][V])
{
    int j, existe=0, k;
    for(j=cidade+1; j < V; j++)
    {
        if(totaldeP + necessidades[j] < soma)
        {
            pos++;
            conjCandidato[pos] = j;
            totaldeP = totaldeP + necessidades[j];
            avaliaItinerarios(j, pos, conjCandidato, totaldeP, indiceConj, lista_itinerarios, necessidades, soma,
                              graph, source, dist, shortestPath);
            totaldeP = totaldeP - necessidades[j];
            conjCandidato[pos] = -1;
            pos--;
        }
        else if (totaldeP + necessidades[j] == soma)
        {
            pos++;
            conjCandidato[pos] = j;

            //existe = verificaIntersecaoItinerarios(lista_itinerarios,conjCandidato,num_itinerarios);

            if(!existe)
            {
                int itinerario_temp[pos+1];
                for (k=0; k <= pos; k++)
                {
                    lista_itinerarios[*indiceConj].cidades_iti[k] = conjCandidato[k];
                    itinerario_temp[k] = conjCandidato[k];
                }
                lista_itinerarios[*indiceConj].indice = *indiceConj;
                lista_itinerarios[*indiceConj].total_P = soma;
                lista_itinerarios[*indiceConj].num_cidades = pos + 1; ///se a ultima posicao preenchida é pos, entao tenho pos+1 cidades

                calculaCustoItinerario(itinerario_temp, graph, source, dist, shortestPath,
                                       lista_itinerarios[*indiceConj].num_cidades, lista_itinerarios, *indiceConj);

                *indiceConj = *indiceConj + 1;
                /*///testa se indiceConj é do tamanho da área atualmente alocada. Em caso afirmativo, realoca
                if(indiceConj == sizeof(lista_itinerarios)/sizeof(lista_itinerarios[0]))
                {
                    realoca(lista_itinerarios);
                }*/

                conjCandidato[pos] = -1;
                pos--;
            }
            else
            {
                conjCandidato[pos] = -1;
                pos--;
            }
        }
    }
}


int retornaCidadeMaisProximaERemoveDoItinerario(int source, int dist[V][V], int itinerario[],
        int size_itinerario, int graph[V][V]) //size_itinerario = num_cidades
{
    int cidade_mais_proxima = -1, i;
    int menor_dist = INT_MAX;
    int pos = 0;

    for (i = 0; i < size_itinerario; i++)
    {
        if(itinerario[i] != -1 && itinerario[i] != source)
        {
            if(dist[source][itinerario[i]] < menor_dist)
            {
                menor_dist = dist[source][itinerario[i]];
                cidade_mais_proxima = itinerario[i];
                pos = i;
            }
            else if(dist[source][itinerario[i]] == menor_dist)
            {
                //verifica se itinerario[i] é adjacente a source e seta uma variavel booleana global
                if(graph[source][itinerario[i]] > 0)
                {
                    menor_dist = dist[source][itinerario[i]];
                    cidade_mais_proxima = itinerario[i];
                    pos = i;
                }
            }
        }
    }
    itinerario[pos] = -1;
    return cidade_mais_proxima;
}

///itinerario é uma cópia de lista_itinerarios[i].cidades_iti
void calculaCustoItinerario(int itinerario[], int graph[V][V], int source, int dist[V][V],
                            int shortestPath[V][V][V], int size_itinerario,
                            struct itinerario* lista_itinerarios, int indice_iti) //, size_itinerario
{
    int custo = 0, vi, vj, j, m;
    int itinerario_menor_custo[size_itinerario+1];///+1 porque A também é inserido no itinerário
    itinerario_menor_custo[0] = source;

    vi = retornaCidadeMaisProximaERemoveDoItinerario(source,dist,itinerario,size_itinerario,graph); //retorna a proxima cidade do itinerario e remove ela do itinerario

    if(vi != -1)
    {
        itinerario_menor_custo[1] = vi;
        custo = dist[source][vi];
        j=1, m=2;

        while(j < size_itinerario) // && itinerario[i] != -1)
        {
            vj = retornaCidadeMaisProximaERemoveDoItinerario(vi,dist,itinerario,size_itinerario,graph);
            if(vj == -1) break;
            custo = custo + dist[vi][vj];
            vi = vj;
            itinerario_menor_custo[m] = vi;
            j++;
            m++;
        }
        custo = custo + dist[0][vi];
    }
    lista_itinerarios[indice_iti].custo = custo;
}

void imprimeDistancias(int dist[V][V])
{
    printf("Distancia da empresa:\n");
    for (int i = 0; i < V; i++)
    {
        printf("%d \t\t %d\n", i, dist[0][i]);
    }
}

void imprimeVetor(int vetor[], int size)
{
    for(int i=0; i < size; i++)
        if(vetor[i] != -1 && vetor[i] != INT_MAX)
            printf(" %d", vetor[i]);
}

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(int dist[V][V], int sptSet[V][V], int source)
{
    // Initialize min value
    int min = INT_MAX, min_index;
    int value;
    for (int v = 0; v < V; v++)
    {
        value = dist[source][v];
        if (sptSet[source][v] == 0 && value <= min)
        {
            min = dist[source][v];
            min_index = v;
        }
    }
    return min_index;
}

// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(int graph[V][V], int source, int dist[V][V], int shortestPath[V][V][V])
{
    //int dist[V];     // The output array.  dist[i] will hold the shortest
    // distance from src to i
    int pred[V][V]; //para cada vértice eu armazeno o predecessor dele no caminho mínimo até a fonte
    int sptSet[V][V]; // sptSet[i] will true if vertex i is included in shortest
    // path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
        dist[source][i] = INT_MAX, sptSet[source][i] = 0, pred[source][i] = -1;

    // Distance of source vertex from itself is always 0
    dist[source][source] = 0;
    // Find shortest path for all vertices
    for (int count = 0; count < V-1; count++)
    {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in first iteration.
        int u = minDistance(dist, sptSet, source);

        // Mark the picked vertex as processed
        sptSet[source][u] = 1;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++)

            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[source][v] && graph[u][v] > 0 && dist[source][u] != INT_MAX
                    && dist[source][u]+graph[u][v] < dist[source][v])
            {
                dist[source][v] = dist[source][u] + graph[u][v];
                pred[source][v] = u;
            }

    }
    int pos, u, j;
    int shortest_path_aux[V];

    for (int i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
        {
            shortest_path_aux[j] = -1;
        }
        //imprime caminho minimo
        pos = 0;
        u = pred[source][i];
        while(u != -1 && u != source)
        {
            shortest_path_aux[pos] = u;
            u = pred[source][u];
            pos++;
        }
        int k =0;
        for(j=pos-1; j >= 0 && shortest_path_aux[j] != -1; j--)
        {
            shortestPath[source][i][k] = shortest_path_aux[j];
            k++;
        }
    }
}

int particiona(struct itinerario* vetor, int inicio, int fim)
{
    int esq, dir;
    // declaração dinâmica de vetores
    struct itinerario pivo, aux;

    esq = inicio;
    dir = fim;
    pivo = vetor[inicio];

    while(esq<dir)
    {
        while(vetor[esq].custo <= pivo.custo && esq<fim)   // vetor[esq] <= pivo
            esq++;

        while((pivo.custo < vetor[dir].custo) == 1)    //  vetor[dir] > pivo
            dir--;

        if(dir < inicio || esq > fim) printf("\nERRO\n\n");

        if(esq < dir)
        {
            aux = vetor[esq];  // troca vetor[esq] com vetor[dir]

            vetor[esq] = vetor[dir];
            vetor[esq].indice = esq;

            vetor[dir] = aux;
            vetor[dir].indice = dir;
        }
    }
    vetor[inicio] = vetor[dir];
    vetor[inicio].indice = inicio;
    vetor[dir] = pivo;
    vetor[dir].indice = dir;
    return dir;                   //retorna dir, que é o índice que vai dividir o vetor
}

void quick(struct itinerario* lista_itinerarios, int inicio, int fim)
{
    int pivo;
    if(fim>inicio)
    {
        pivo = particiona(lista_itinerarios,inicio,fim);         // encontra um pivo que "divide" o vetor em dois
        quick(lista_itinerarios, inicio, pivo-1);                // realiza novamente para a parte da esquerda
        quick(lista_itinerarios, pivo+1, fim);                   // e realiza para a parte de direita
    }
}

int particiona_solucoes(struct solucao* vetor, int inicio, int fim)
{
    int esq, dir;
    // declaração dinâmica de vetores
    struct solucao pivo, aux;

    esq = inicio;
    dir = fim;
    pivo = vetor[inicio];

    while(esq<dir)
    {
        while(vetor[esq].custo_total <= pivo.custo_total && esq<fim)   // vetor[esq] <= pivo
            esq++;

        while((pivo.custo_total < vetor[dir].custo_total) == 1)    //  vetor[dir] > pivo
            dir--;

        if(dir < inicio || esq > fim) printf("\nERRO\n\n");

        if(esq < dir)
        {
            aux = vetor[esq];  // troca vetor[esq] com vetor[dir]

            vetor[esq] = vetor[dir];
            //vetor[esq].indice = esq;

            vetor[dir] = aux;
            //vetor[dir].indice = dir;
        }
    }
    vetor[inicio] = vetor[dir];

    vetor[dir] = pivo;

    return dir;                   //retorna dir, que é o índice que vai dividir o vetor
}

void quick_solucoes(struct solucao* solucoes, int inicio, int fim)
{
    int pivo;
    if(fim>inicio)
    {
        pivo = particiona_solucoes(solucoes,inicio,fim);         // encontra um pivo que "divide" o vetor em dois
        quick_solucoes(solucoes, inicio, pivo-1);                // realiza novamente para a parte da esquerda
        quick_solucoes(solucoes, pivo+1, fim);                   // e realiza para a parte de direita
    }
}

int particiona_int(int vetor[], int inicio, int fim)
{
    int esq, dir;
    // declaração dinâmica de vetores
    int pivo, aux;

    esq = inicio;
    dir = fim;
    pivo = vetor[inicio];

    while(esq<dir)
    {
        while(vetor[esq] <= pivo && esq<fim)   // vetor[esq] <= pivo
            esq++;

        while(pivo < vetor[dir] == 1)    //  vetor[dir] > pivo
            dir--;

        if(dir < inicio || esq > fim) printf("\nERRO\n\n");

        if(esq < dir)
        {
            aux = vetor[esq];          // troca vetor[esq] com vetor[dir]
            vetor[esq] = vetor[dir];
            vetor[dir] = aux;
        }
    }
    vetor[inicio] = vetor[dir];
    vetor[dir] = pivo;
    return dir;                   //retorna dir, que é o índice que vai dividir o vetor
}

void quick_int(int vetor[], int inicio, int fim)
{
    int pivo;
    if(fim>inicio)
    {
        pivo = particiona_int(vetor,inicio,fim);         // encontra um pivo que "divide" o vetor em dois
        quick_int(vetor, inicio, pivo-1);                // realiza novamente para a parte da esquerda
        quick_int(vetor, pivo+1, fim);                   // e realiza para a parte de direita
    }
}

/// Verifica se há alguma cidade do itinerário na lista de cidades já escolhidas
int verificaCidadesEscolhidas(int cidades_iti[], int cidades_escolhidas[], int size_iti)
{
    for(int i=0; i < size_iti; i++)
    {
        if(cidades_escolhidas[cidades_iti[i]] == 1)
            return 1;
    }
    return 0;
}

/// Marca as cidades do itinerário dado como não escolhidas
void resetCidadesEscolhidas(int cidades_iti[], int cidades_escolhidas[], int size_iti)
{
    for(int i=0; i < size_iti; i++)
        cidades_escolhidas[cidades_iti[i]] = 0;
}

/// A cada iteração, seleciona um conjunto de candidatos viaveis que não possuem cidades em comum
/// e adiciona eles à matriz combinacaoItinerarios

///1ª chamada: indice = j, pos = 0, total_de_escolhas = 0, indiceConj = 0
void selecionaCandidatos(struct itinerario* lista_itinerarios, int indice, int pos, int conjCandidato[],
                         int total_de_escolhas, int *indiceConj, int **combinacaoItinerarios,
                         int tam_frota, int num_itinerarios, int cidades_escolhidas_temp[], int lista_invalidas[],
                         int novo_num_itinerarios)
{
    int j, k, l, pos_iti, cont = 0, existe = 0;

    for(j=indice; j < num_itinerarios; j++)
    {
        invalidaItinerarios(lista_itinerarios, lista_invalidas, num_itinerarios, V-1, -1);
        //pos_iti = buscaPosProxItinerarioValido(j,lista_itinerarios,num_itinerarios);
        //if(pos_iti != -1) /// Se foi encontrado algum itinerario acrescenta ele ao conjunto solução
        ///Verifica se o itinerario atual é válido
        if(lista_itinerarios[j].indice != -1 && lista_itinerarios[j].invalido == 0)
        {
            ///Verifica se alguma cidade do itinerário escolhido já foi escolhida anteriormente para esse candidato à solução
            if(verificaCidadesEscolhidas(lista_itinerarios[j].cidades_iti, cidades_escolhidas_temp,
                                         lista_itinerarios[j].num_cidades) == 0)
            {

                pos_iti = j;
                conjCandidato[pos] = lista_itinerarios[pos_iti].indice;
                pos++;

                /// depois de escolher um itinerário eu preciso invalidar todos os outros itinerários que
                /// possuem alguma cidade em comum com o itinerário que acabou de ser escolhido
                for(l=0; l < lista_itinerarios[pos_iti].num_cidades; l++)
                {
                    ///adiciona a lista de cidades no vetor cidades_escolhidas
                    cidades_escolhidas_temp[lista_itinerarios[pos_iti].cidades_iti[l]] = 1;
                    cont++;
                }

                invalidaItinerarios(lista_itinerarios,lista_itinerarios[pos_iti].cidades_iti,num_itinerarios,
                                    lista_itinerarios[pos_iti].num_cidades, pos_iti);

                total_de_escolhas++;
                if(total_de_escolhas == tam_frota)
                {
                    /// Verifica se a sequência de itinerários já não existe
                    existe = verificaSeJaExisteConj(combinacaoItinerarios,conjCandidato,
                                                    novo_num_itinerarios,tam_frota);
                    if(!existe)
                    {
                        /// Uma última etapa após encontrar uma combinação é opcionalmente verificar
                        /// se a combinação atende a uma lista de cidades que devem ser atendidas pelas
                        /// restrições impostas ao problema
                        for (l=0; l < tam_frota; l++)
                        {
                            combinacaoItinerarios[*indiceConj][l] = conjCandidato[l];
                        }
                        *indiceConj = *indiceConj + 1;

                        pos--;
                        conjCandidato[pos] = -1;
                        total_de_escolhas--;
                    }
                    else
                    {
                        pos--;
                        conjCandidato[pos] = -1;
                        total_de_escolhas--;
                    }
                    validaItinerarios(lista_itinerarios,lista_itinerarios[pos_iti].cidades_iti,num_itinerarios,
                                      lista_itinerarios[pos_iti].num_cidades);
                    resetCidadesEscolhidas(lista_itinerarios[pos_iti].cidades_iti,cidades_escolhidas_temp,
                                           lista_itinerarios[pos_iti].num_cidades);

                }
                else ///Continua a construir o conjunto solução recursivamente
                {
                    selecionaCandidatos(lista_itinerarios, j+1, pos, conjCandidato, total_de_escolhas,
                                        indiceConj, combinacaoItinerarios, tam_frota, num_itinerarios, cidades_escolhidas_temp,
                                        lista_invalidas, novo_num_itinerarios
                                       );
                    pos--;
                    conjCandidato[pos] = -1;
                    total_de_escolhas--;
                    validaItinerarios(lista_itinerarios,lista_itinerarios[pos_iti].cidades_iti,num_itinerarios,
                                      lista_itinerarios[pos_iti].num_cidades);
                    resetCidadesEscolhidas(lista_itinerarios[pos_iti].cidades_iti,cidades_escolhidas_temp,
                                           lista_itinerarios[pos_iti].num_cidades);
                }
            }
        }
        //else break;
    }
}

int verificaListaPreenchida(int lista[], int size_lista, int valor_invalido)
{
    int cont = 0;
    for(int i=0; i < size_lista; i++)
        if(lista[i] != valor_invalido)
            cont++;
    if (cont == size_lista)
        return 1;
    else
        return 0;
}

/// A cada iteração, seleciona um conjunto de candidatos viaveis que não possuem cidades em comum
/// e adiciona eles à matriz combinacaoItinerarios

///1ª chamada: indice = j, pos = 0, total_de_escolhas = 0, indiceConj = 0
void selecionaCandidatos2(struct itinerario* lista_itinerarios, int indice, int pos, int conjCandidato[],
                           int total_de_escolhas, int *indiceConj, int **combinacaoItinerarios,
                           int tam_frota, int num_itinerarios, int cidades_escolhidas_temp[], int lista_invalidas[],
                           int cidades_escolhidas[], int novo_num_itinerarios)
{
    //int total_de_escolhas = 0;
    int j, k, l, pos_iti, cont = 0, existe = 0;
    int restantes[V];

    for(j=indice; j < num_itinerarios; j++)
    {
        invalidaItinerarios(lista_itinerarios, lista_invalidas, num_itinerarios, V-1, -1);
        //pos_iti = buscaPosProxItinerarioValido(j,lista_itinerarios,num_itinerarios);
        //if(pos_iti != -1) /// Se foi encontrado algum itinerario acrescenta ele ao conjunto solução
        ///Verifica se o itinerario atual é válido
        if(lista_itinerarios[j].indice != -1 && lista_itinerarios[j].invalido == 0)
        {
            ///Verifica se alguma cidade do itinerário escolhido já foi escolhida anteriormente para esse candidato à solução
            if(verificaCidadesEscolhidas(lista_itinerarios[j].cidades_iti, cidades_escolhidas,
                                         lista_itinerarios[j].num_cidades) == 0)
            {

                pos_iti = j;
                conjCandidato[pos] = lista_itinerarios[pos_iti].indice;
                pos++;

                /// depois de escolher um itinerário eu preciso invalidar todos os outros itinerários que
                /// possuem alguma cidade em comum com o itinerário que acabou de ser escolhido
                for(l=0; l < lista_itinerarios[pos_iti].num_cidades; l++)
                {
                    ///adiciona a lista de cidades no vetor cidades_escolhidas
                    cidades_escolhidas_temp[lista_itinerarios[pos_iti].cidades_iti[l]] = 1;
                    cidades_escolhidas[lista_itinerarios[pos_iti].cidades_iti[l]] = 1;
                    cont++;
                }

                invalidaItinerarios(lista_itinerarios,lista_itinerarios[pos_iti].cidades_iti,num_itinerarios,
                                    lista_itinerarios[pos_iti].num_cidades, pos_iti);

                total_de_escolhas++;
                ///Verifica se eu já escolhi todas as cidades. Em caso negativo, verifica se essa foi a última escolha
                if(verificaListaPreenchida(cidades_escolhidas,V,0) == 1)
                {
                    existe = verificaSeJaExisteConj(combinacaoItinerarios,conjCandidato,
                                                    combinacao(novo_num_itinerarios,tam_frota),tam_frota);
                    if(!existe)
                    {
                        for (l=0; l < tam_frota; l++)
                        {
                            combinacaoItinerarios[*indiceConj][l] = conjCandidato[l];
                        }
                        *indiceConj = *indiceConj + 1;
                        pos--;
                        conjCandidato[pos] = -1;
                        total_de_escolhas--;
                    }
                    else
                    {
                        pos--;
                        conjCandidato[pos] = -1;
                        total_de_escolhas--;
                    }
                    validaItinerarios(lista_itinerarios,lista_itinerarios[pos_iti].cidades_iti,num_itinerarios,
                                      lista_itinerarios[pos_iti].num_cidades);
                    resetCidadesEscolhidas(lista_itinerarios[pos_iti].cidades_iti,cidades_escolhidas_temp,
                                           lista_itinerarios[pos_iti].num_cidades);
                    resetCidadesEscolhidas(lista_itinerarios[pos_iti].cidades_iti,cidades_escolhidas,
                                           lista_itinerarios[pos_iti].num_cidades);
                }
                else if(total_de_escolhas == tam_frota)
                {
                    /// Verifica se a sequência de itinerários já não existe
                    existe = verificaSeJaExisteConj(combinacaoItinerarios,conjCandidato,
                                                    combinacao(novo_num_itinerarios,tam_frota),tam_frota);
                    if(!existe)
                    {
                        /// Uma última etapa após encontrar uma combinação é opcionalmente verificar
                        /// se a combinação atende a uma lista de cidades que devem ser atendidas pelas
                        /// restrições impostas ao problema
                        for (l=0; l < tam_frota; l++)
                        {
                            combinacaoItinerarios[*indiceConj][l] = conjCandidato[l];
                        }
                        *indiceConj = *indiceConj + 1;
                        pos--;
                        conjCandidato[pos] = -1;
                        total_de_escolhas--;
                    }
                    else
                    {
                        pos--;
                        conjCandidato[pos] = -1;
                        total_de_escolhas--;
                    }
                    validaItinerarios(lista_itinerarios,lista_itinerarios[pos_iti].cidades_iti,num_itinerarios,
                                      lista_itinerarios[pos_iti].num_cidades);
                    resetCidadesEscolhidas(lista_itinerarios[pos_iti].cidades_iti,cidades_escolhidas_temp,
                                           lista_itinerarios[pos_iti].num_cidades);
                    resetCidadesEscolhidas(lista_itinerarios[pos_iti].cidades_iti,cidades_escolhidas,
                                           lista_itinerarios[pos_iti].num_cidades);

                }
                else ///Continua a construir o conjunto solução recursivamente
                {
                    selecionaCandidatos2(lista_itinerarios, j+1, pos, conjCandidato, total_de_escolhas,
                                        indiceConj, combinacaoItinerarios, tam_frota, num_itinerarios,
                                        cidades_escolhidas_temp, lista_invalidas, cidades_escolhidas, novo_num_itinerarios
                                       );
                    pos--;
                    conjCandidato[pos] = -1;
                    total_de_escolhas--;
                    validaItinerarios(lista_itinerarios,lista_itinerarios[pos_iti].cidades_iti,num_itinerarios,
                                    lista_itinerarios[pos_iti].num_cidades);
                    resetCidadesEscolhidas(lista_itinerarios[pos_iti].cidades_iti,cidades_escolhidas_temp,
                                           lista_itinerarios[pos_iti].num_cidades);
                    resetCidadesEscolhidas(lista_itinerarios[pos_iti].cidades_iti,cidades_escolhidas,
                                           lista_itinerarios[pos_iti].num_cidades);
                }
            }
        }
        //else break;
    }
}


void retornaCidadesRestantes(int cidades_escolhidas[], int restantes[])
{
    int ind = 0, i;
    for(int i=0; i < V; i++)
        restantes[i] = -1;

    for(i=1; i < V; i++)
        if(cidades_escolhidas[i] == 0)
        {
            restantes[ind] = i;
            ind++;
        }
}

void imprimeItinerario(struct itinerario iti)
{
    int i,j;
    if(iti.indice != -1)
    {
        printf("\n-------//---------\n");
        printf("Itinerário:\n");
        printf("Custo do itinerario = %d, Itinerario: ", iti.custo);
        for(j=0; j < iti.num_cidades; j++)
            printf("%c ",  cidades[iti.cidades_iti[j]]);
        printf(", Nº de cidades: %d, Indice: %d", iti.num_cidades,iti.indice);
        printf("\n");
        printf("\n-------//---------\n");
    }
}

void imprimeListaItinerarios(struct itinerario* lista_itinerarios, int size_lista)
{
    int i,j;
    printf("\n----------------\n");
    printf("Itinerários:\n");
    for(i=0; i < size_lista; i++)
    {
        if(lista_itinerarios[i].indice != -1 && lista_itinerarios[i].invalido == 0)
        {
            printf("Custo do itinerario = %d, Itinerario: ", lista_itinerarios[i].custo);
            for(j=0; j < lista_itinerarios[i].num_cidades; j++)
                printf("%c ",  cidades[lista_itinerarios[i].cidades_iti[j]]);
            printf(", Nº de cidades: %d, Indice: %d", lista_itinerarios[i].num_cidades,lista_itinerarios[i].indice);
            printf("\n");
        }
    }
    printf("\n----------------\n");
}

void imprimeCombinacoes(int **combinacoes, int size_matriz, int tam)
{
    int m,p;
    printf("\n---------------//---------------------\n");
    printf("Combinaçoes validas para essa iteração:\n");
    for(m=0; m < size_matriz; m++)
    {
        for(p=0; p < tam; p++)
        {
            printf("%d ", combinacoes[m][p]);
        }
        printf("\n");
    }
    printf("\n---------------//---------------------\n");
}

int main()
{
    int graph[V][V] = {{0, 10, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {10, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 5, 0, 7, 3, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 7, 0, 3, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 4, 6, 8, 0, 0, 0},
        {0, 0, 0, 0, 0, 4, 0, 2, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 6, 2, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 8, 0, 0, 0, 15, 2, 3},
        {0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0}
    };
    int necessidades[V] = {0,7,4,3,5,2,3,1,5,4,1,2};
    //int necessidades[V] = {0,5,4,3,5,2,3,1,5,4,1,2};
    //int necessidades[V] = {0,5,4,3,5,2,3,1,5,1,1,2};
    int i,j,k,m,p;
    int source = 0;
    int shortestPath[V][V][V];
    int dist[V][V];
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
        {
            dist[i][j] = INT_MAX;
            for (k = 0; k < V; k++)
            {
                shortestPath[k][i][j] = -1;
            }
        }

    int src;
    for (src=0; src < V; src++)
        dijkstra(graph,src,dist,shortestPath);
    imprimeDistancias(dist);

    int listaCidadesInvalidasCam3P[V];
    int listaCidadesInvalidasCam5P[V];
    for(i=0; i < V; i++)
    {
        listaCidadesInvalidasCam3P[i] = -1;
        listaCidadesInvalidasCam5P[i] = -1;
    }

    int pos3P = 0, pos5P = 0;
    for(i=0; i < V; i++)
    {
        if(necessidades[i] > 3)
        {
            listaCidadesInvalidasCam3P[pos3P] = i;
            pos3P++;
        }
        if(necessidades[i] > 5)
        {
            listaCidadesInvalidasCam5P[pos5P] = i;
            pos5P++;
        }
    }

    printf("Lista de cidades inválidas para cada tipo de caminhão:\n");
    printf("3P:");
    for(i=0; i < V; i++)
    {
        if (listaCidadesInvalidasCam3P[i] == -1) break;
        printf("%c ", cidades[listaCidadesInvalidasCam3P[i]]);
    }
    printf("\n5P:");
    for(i=0; i < V; i++)
    {
        if (listaCidadesInvalidasCam5P[i] == -1) break;
        printf("%c ", cidades[listaCidadesInvalidasCam5P[i]]);
    }

    lista_itinerarios_10P = (struct itinerario*)malloc(MAX*sizeof(struct itinerario));
    lista_itinerarios_5P = (struct itinerario*)malloc(MAX*sizeof(struct itinerario));
    lista_itinerarios_3P = (struct itinerario*)malloc(MAX*sizeof(struct itinerario));
    for(i=0; i < MAX; i++)
    {
        lista_itinerarios_10P[i].cidades_iti = (int*)malloc(5*sizeof(int));
        lista_itinerarios_5P[i].cidades_iti = (int*)malloc(5*sizeof(int));
        lista_itinerarios_3P[i].cidades_iti = (int*)malloc(5*sizeof(int));
        ///inicializa indice com -1
        for(j=0; j < 5; j++)
        {
            lista_itinerarios_10P[i].cidades_iti[j] = -1;
            lista_itinerarios_5P[i].cidades_iti[j] = -1;
            lista_itinerarios_3P[i].cidades_iti[j] = -1;
        }
        lista_itinerarios_10P[i].custo = -1;
        lista_itinerarios_5P[i].custo = -1;
        lista_itinerarios_3P[i].custo = -1;

        lista_itinerarios_5P[i].invalido = 0;
        lista_itinerarios_3P[i].invalido = 0;
    }

    int conjCandidato[] = {-1,-1,-1,-1,-1};

    int totaldeP=0, indiceConj=0, cidade;
    int pos;
    int soma;
    ///Para o caminhão de 10P, calcula todos os itinerários (conjuntos de cidades) incluindo a cidade B cujas necessidades das cidades somam 7, 8, 9 ou 10 produtos
    for(soma = 6; soma <= 10; soma++)
    {
        for(cidade=1; cidade < V; cidade++)
        {
            for (k=0; k < 5; k++)
                conjCandidato[k] = -1; ///-1 sempre representa um valor inválido no meu código
            pos = 0;
            conjCandidato[pos] = cidade;
            totaldeP = necessidades[cidade];
            if(necessidades[cidade] == soma) //conjunto com uma única cidade
            {
                lista_itinerarios_10P[indiceConj].cidades_iti[0] = cidade;
                lista_itinerarios_10P[indiceConj].indice = indiceConj;
                lista_itinerarios_10P[indiceConj].total_P = soma;
                lista_itinerarios_10P[indiceConj].num_cidades = 1;

                int itinerario_temp[1];
                itinerario_temp[0] = cidade;
                calculaCustoItinerario(itinerario_temp, graph, source, dist, shortestPath,
                1, lista_itinerarios_10P, indiceConj);
                indiceConj++;
            }
            else if (cidade != V-1)
                avaliaItinerarios(cidade, pos, conjCandidato, totaldeP, &indiceConj,
                lista_itinerarios_10P, necessidades, soma,
                graph, 0, dist, shortestPath);
        }
    }

    int NUM_ITI_3P, NUM_ITI_5P, NUM_ITI_10P = indiceConj;
    printf("NUM_ITI_10P: %d\n", NUM_ITI_10P);
    printf("Itinerários 10P:\n");
    quick(lista_itinerarios_10P,0,NUM_ITI_10P-1);

    int new_NUM_ITI_10P = filtraItinerarios(lista_itinerarios_10P, listaCidadesInvalidasCam5P, NUM_ITI_10P, V);
    ///Criar novo vetor com os itinerários filtrados


    printf("Itinerários ordenados por custo:\n");
    imprimeListaItinerarios(lista_itinerarios_10P,NUM_ITI_10P);

    //int custoCompartilhado[V][V];
    //retornaCustoCompartilhado(graph, 0, custoCompartilhado);

    indiceConj = 0;
    ///Para um caminhão de 5P, calcula todos os itinerários (conjuntos de cidades) cujas necessidades das cidades somam 4 ou 5 produtos
    for(soma = 4; soma <= 5; soma++)
    {
        for(cidade=1; cidade < V; cidade++)
        {
            for (k=0; k < 5; k++)
                conjCandidato[k] = -1; ///-1 sempre representa um valor inválido no meu código
            pos = 0;
            conjCandidato[pos] = cidade;
            totaldeP = necessidades[cidade];
            if(necessidades[cidade] == soma) //conjunto com uma única cidade
            {
                lista_itinerarios_5P[indiceConj].cidades_iti[0] = cidade;
                lista_itinerarios_5P[indiceConj].indice = indiceConj;
                lista_itinerarios_5P[indiceConj].num_cidades = 1;
                int itinerario_temp[1];
                itinerario_temp[0] = cidade;
                calculaCustoItinerario(itinerario_temp, graph, source, dist, shortestPath,
                                       1, lista_itinerarios_5P, indiceConj);

                indiceConj++;
            }
            else if (cidade != V-1)
                avaliaItinerarios(cidade, pos, conjCandidato, totaldeP, &indiceConj,
                                  lista_itinerarios_5P, necessidades, soma,
                                  graph, 0, dist, shortestPath);
        }
    }

    NUM_ITI_5P = indiceConj;

    int nomesCidades[] = {2,4,8,9};

    quick(lista_itinerarios_5P,0,NUM_ITI_5P-1);

    int new_num_iti_5P = 0;  ///funcionar com listaInvalidas3P
    //new_num_iti_5P = filtraItinerarios(lista_itinerarios_5P, nomesCidades, NUM_ITI_5P, 4);
    new_num_iti_5P = filtraItinerarios(lista_itinerarios_5P, listaCidadesInvalidasCam3P, NUM_ITI_5P, V);
    printf("Novo numero de iti 5P: %d\n", new_num_iti_5P);

    printf("Itinerários 5P após filtrar\n");
    imprimeListaItinerarios(lista_itinerarios_5P,NUM_ITI_5P);

    int tam_frota_5P = 3;
    ///Novo número de itinerários
    int numItinerarios5P = combinacao(new_num_iti_5P,tam_frota_5P);

    printf("Num de combinações após filtrar itinerários %d\n", numItinerarios5P);
    ///Calcula todas as combinações possíveis dessa lista de itinerários obtida

    int **combinacoesIti5P = (int**)malloc(numItinerarios5P*sizeof(int*));
    for(i=0; i < numItinerarios5P; i++)
        combinacoesIti5P[i] = (int*)malloc(tam_frota_5P*sizeof(int)); ///4 caminhoes 5P

    for(i=0; i < numItinerarios5P; i++)
        for(j=0; j < tam_frota_5P; j++)
            combinacoesIti5P[i][j] = -1;

    indiceConj = 0;
    ///Para um caminhão de 3P, calcula todos os itinerários (conjuntos de cidades) cujas necessidades das cidades somam 1, 2 ou 3 produtos
    for(soma = 2; soma <= 3; soma++)
    {
        for(cidade=1; cidade < V; cidade++)
        {
            for (k=0; k < 5; k++)
                conjCandidato[k] = -1; ///-1 sempre representa um valor inválido no meu código
            pos = 0;
            conjCandidato[pos] = cidade;
            totaldeP = necessidades[cidade];
            if(necessidades[cidade] == soma) //conjunto com uma única cidade
            {
                lista_itinerarios_3P[indiceConj].cidades_iti[0] = cidade;
                lista_itinerarios_3P[indiceConj].indice = indiceConj;
                lista_itinerarios_3P[indiceConj].num_cidades = 1;
                int itinerario_temp[1];
                itinerario_temp[0] = cidade;
                calculaCustoItinerario(itinerario_temp, graph, source, dist, shortestPath,
                                       1, lista_itinerarios_3P, indiceConj);

                indiceConj++;
            }
            else if (cidade != V-1)
                avaliaItinerarios(cidade, pos, conjCandidato, totaldeP, &indiceConj,
                                  lista_itinerarios_3P, necessidades, soma,
                                  graph, 0, dist, shortestPath);
        }
    }

    NUM_ITI_3P = indiceConj;

    quick(lista_itinerarios_3P,0,NUM_ITI_3P-1);

    printf("NUM_ITI_3P: %d\n", NUM_ITI_3P);
    printf("\n-------//---------\n");
    printf("Itinerários 3P:\n");
    imprimeListaItinerarios(lista_itinerarios_3P,NUM_ITI_3P);

    int tam_frota_3P = 5;
    int numItinerarios3P = combinacao(NUM_ITI_3P,tam_frota_3P);


    int **combinacoesIti3P = (int**)malloc(numItinerarios3P*sizeof(int*));
    for(i=0; i < numItinerarios3P; i++)
        combinacoesIti3P[i] = (int*)malloc(tam_frota_3P*sizeof(int)); ///4 caminhoes 5P
    for(i=0; i < numItinerarios3P; i++)
        for(j=0; j < tam_frota_3P; j++)
            combinacoesIti3P[i][j] = -1;

    int total_escolhas = NUM_ITI_10P*numItinerarios5P*numItinerarios3P;
    struct solucao* solucoes = (struct solucao*) malloc(total_escolhas*sizeof(struct solucao));

    for(i=0; i < total_escolhas; i++)
    {
        solucoes[i].lista_itinerarios = (struct itinerario*) malloc(10*sizeof(struct itinerario));///10 é o tamanho da frota
        solucoes[i].custo_total = 0;
        solucoes[i].num_caminhoes = 0;
    }

    indiceConj = 0;
    int lista_invalidas[V-1], cont_inv = 0, cidades_escolhidas[V], cidades_escolhidas_temp[V],
                              ind_solucoes = 0, l, ind_invalida_5P, ind_invalida_3P, indice_cam = 0;
    /// Á medida que as soluções vão sendo avaliadas, faz a combinação delas.
    /// Para formar candidatos viáveis:
    /// . Durante a realização das combinações, invalida todas as cidades já escolhidas
    /// . Só acrescenta à solução um itinerário que não possui nenhuma cidade em comum com a lista de cidades já escolhidas
    /// Mantém a lista de cidades que já foram escolhidas para a soluçao atual em cidades_escolhidas
    for (i=0; i < NUM_ITI_10P; i++)
    {
        cont_inv = 0;
        for (p=0; p < V-1; p++)
            lista_invalidas[p] = INT_MAX;

        cidades_escolhidas[0] = 1; ///Seta cidade A como escolhida
        //inicializa com 0 o resto
        for (p=1; p < V; p++)
            cidades_escolhidas[p] = 0;

        printf("Cidades escolhidas:\n");
        for(p=0; p < lista_itinerarios_10P[i].num_cidades; p++)
        {
            printf("%d ", lista_itinerarios_10P[i].cidades_iti[p]);
            lista_invalidas[cont_inv] = lista_itinerarios_10P[i].cidades_iti[p];
            cidades_escolhidas[lista_invalidas[cont_inv]] = 1;
            cont_inv++;
        }
        printf("\n");
        ind_invalida_5P = cont_inv;

        /// new_num_iti_5P: nº de itinerários para o caminhão de 5P após filtrar os itinerários com C, L, G e H
        /// new_num_iti_5P - tam_frota_5P
        for (j=0; j <= new_num_iti_5P - tam_frota_5P; j++)
        {
            ///Reinicializa estruturas
            indiceConj = 0;
            for(p=0; p < numItinerarios5P; p++)
                for(l=0; l < tam_frota_5P; l++)
                    combinacoesIti5P[p][l] = -1;
            for(p=0; p < 5; p++)
                conjCandidato[p] = -1;
            for(p=0; p < V; p++)
                cidades_escolhidas_temp[p] = 0;

            for(p=ind_invalida_5P; p < V-1; p++)
                lista_invalidas[p] = INT_MAX;
            cont_inv = ind_invalida_5P;

            ///reset atributo invalido
            resetInvalidos(lista_itinerarios_5P,NUM_ITI_5P);

            //imprimeVetor(lista_invalidas,V-1);
            ///invalida itinerários com lista_invalidas preenchida a partir da escolha do caminhão de 10P
            invalidaItinerarios(lista_itinerarios_5P, lista_invalidas, NUM_ITI_5P, V-1, -1);

            imprimeListaItinerarios(lista_itinerarios_5P,NUM_ITI_5P);
            ///só coloca no conjunto solução, os itinerários com o atributo invalido == 0
            ///selecionaCandidatos
            selecionaCandidatos2(lista_itinerarios_5P,j,0,conjCandidato,0,&indiceConj,combinacoesIti5P
                                ,tam_frota_5P,NUM_ITI_5P, cidades_escolhidas_temp, lista_invalidas,
                                cidades_escolhidas, new_num_iti_5P);

            /// Guarda cada combinação retornada no vetor solucoes com as informações de custo total,
            /// lista de itinerários, num_caminhoes
            printf("\n-------//---------\n");
            printf("Combinaçoes de itinerarios 5P validas para essa iteração:\n");
            imprimeCombinacoes(combinacoesIti5P,numItinerarios5P,tam_frota_5P);
            ///*Guadar a ultima posicao de combinacao valida para otimizar o for abaixo

            ///Para cada itinerário 5P válido, acha as combinações de itinerários 3P válidas
            ///Preencha o vetor solucoes somente quando for testar cada combinacao 3P
            for(m=0; m < numItinerarios5P; m++)
            {
                if(combinacoesIti5P[m][0] == -1) break; ///Verifica se é uma linha da matriz que está preenchida
                else
                {
//                    for(p=ind_invalida_5P; p < V-1; p++)
//                        lista_invalidas[p] = INT_MAX;
//                    cont_inv = ind_invalida_5P;
                    for(p=0; p < tam_frota_5P; p++)
                    {
                        struct itinerario iti_temp = buscaItinerario(combinacoesIti5P[m][p],lista_itinerarios_5P,numItinerarios5P);
                        for(l=0; l < iti_temp.num_cidades; l++)
                        {
                            lista_invalidas[cont_inv] = iti_temp.cidades_iti[l];
                            cidades_escolhidas[lista_invalidas[cont_inv]] = 1;
                            cont_inv++;
                        }
                    }
                    ind_invalida_3P = cont_inv;
                    ///Código que escolhe itinerários 3P  incrementa ind_solucoes
                    ///Na seleção de um candidato 3P, verifique se a solução atende a todas as cidades.
                    ///Para isso, utilize lista_invalidas.   Verificar lista_invalidas toda preenchida?
                    ///selecionaCandidatos3P: só guarda no vetor combinacoes se a solução atender a todas as cidades
                    //for(k=0; k < NUM_ITI_3P; k++)
                    for(k=0; k <= NUM_ITI_3P - tam_frota_3P; k++)
                    {
                        for(p=ind_invalida_3P; p < V-1; p++)
                            lista_invalidas[p] = INT_MAX;
                        cont_inv = ind_invalida_3P;

                        printf("Lista de cidades já escolhidas:\n");
                        imprimeVetor(lista_invalidas,V-1);

                        ///----------------//---------------------
                        ///Começa o teste de itinerários 3P
                        ///seria melhor fazer a combinação de itinerários 3P a partir da lista de cidades restantes
                        //ind_invalida_3P = cont_inv;
                        ///Reinicializa estruturas
                        indiceConj = 0;
                        for(p=0; p < numItinerarios3P; p++)
                            for(l=0; l < tam_frota_3P; l++)
                                combinacoesIti3P[p][l] = -1;
                        for(p=0; p < 5; p++)
                            conjCandidato[p] = -1;
                        for(p=0; p < V; p++)
                            cidades_escolhidas_temp[p] = 0;

                        ///reset atributo invalido
                        resetInvalidos(lista_itinerarios_3P,NUM_ITI_3P);

                        //imprimeVetor(lista_invalidas,V-1);
                        ///invalida itinerários com lista_invalidas preenchida a partir das escolhas para 10P e 5P
                        invalidaItinerarios(lista_itinerarios_3P, lista_invalidas, NUM_ITI_3P, V-1, -1);

                        //imprimeListaItinerarios(lista_itinerarios_3P,NUM_ITI_3P);
                        ///só coloca no conjunto solução, os itinerários com o atributo invalido == 0
                        ///selecionaCandidatos
                        selecionaCandidatos2(lista_itinerarios_3P,k,0,conjCandidato,0,&indiceConj,combinacoesIti3P
                                              ,tam_frota_3P,NUM_ITI_3P, cidades_escolhidas_temp, lista_invalidas,
                                              cidades_escolhidas, NUM_ITI_3P);
//
//                        /// Guarda cada combinação retornada no vetor solucoes com as informações de custo total,
//                        /// lista de itinerários, num_caminhoes
                        printf("\n-------//---------\n");
                        printf("Combinaçoes de itinerarios 3P validas para essa iteração:\n");
                        //imprimeCombinacoes(combinacoesIti3P,numItinerarios3P,tam_frota_3P);

                        for(int z=0; z < numItinerarios3P; z++)
                        {
                            if(combinacoesIti3P[z][0] == -1) break;
                            else
                            {
//                                for(p=ind_invalida_3P; p < V-1; p++)
//                                    lista_invalidas[p] = INT_MAX;
//                                cont_inv = ind_invalida_3P;

                                for(p=0; p < tam_frota_3P; p++)
                                {
                                    if(combinacoesIti3P[z][p] != -1)
                                    {
                                        struct itinerario iti_temp = buscaItinerario(combinacoesIti3P[z][p],
                                                                lista_itinerarios_3P,NUM_ITI_3P);
                                        for(l=0; l < iti_temp.num_cidades; l++)
                                        {
                                            lista_invalidas[cont_inv] = iti_temp.cidades_iti[l];
                                            cidades_escolhidas[lista_invalidas[cont_inv]] = 1;
                                            cont_inv++;
                                        }
                                    }
                                }
                                int restantes[V];
                                retornaCidadesRestantes(cidades_escolhidas,restantes);
                                printf("Cidades restantes\n");
                                imprimeVetor(restantes,V);

                                if(verificaListaPreenchida(cidades_escolhidas,V,0) == 1)
                                {
                                    indice_cam = 0; ///indice do itinerário colocado na solução
                                    ///Armazena as soluções quando estiver escolhendo os itinerários 3P
                                    ///Armazena o itinerário 10P escolhido
                                    solucoes[ind_solucoes].lista_itinerarios[indice_cam] = lista_itinerarios_10P[i];
                                    solucoes[ind_solucoes].custo_total += solucoes[ind_solucoes].lista_itinerarios[indice_cam].custo;
                                    solucoes[ind_solucoes].num_caminhoes += 1;
                                    imprimeItinerario(solucoes[ind_solucoes].lista_itinerarios[indice_cam]);
                                    indice_cam++;
                                    for(p=0; p < tam_frota_5P; p++)
                                    {
                                        ///Armazena os itinerários 5P escolhidos
                                        solucoes[ind_solucoes].lista_itinerarios[indice_cam] = buscaItinerario(combinacoesIti5P[m][p],lista_itinerarios_5P, NUM_ITI_5P);
                                        solucoes[ind_solucoes].custo_total += solucoes[ind_solucoes].lista_itinerarios[indice_cam].custo;
                                        solucoes[ind_solucoes].num_caminhoes += 1;
                                        imprimeItinerario(solucoes[ind_solucoes].lista_itinerarios[indice_cam]);
                                        indice_cam++;
                                    }
                                    for(p=0; p < tam_frota_3P; p++)
                                    {
                                        if(combinacoesIti3P[z][p] != -1)
                                        {
                                            ///Armazena os itinerários 3P escolhidos
                                            solucoes[ind_solucoes].lista_itinerarios[indice_cam] = buscaItinerario(combinacoesIti3P[z][p],lista_itinerarios_3P, NUM_ITI_3P);
                                            solucoes[ind_solucoes].custo_total += solucoes[ind_solucoes].lista_itinerarios[indice_cam].custo;
                                            solucoes[ind_solucoes].num_caminhoes += 1;
                                            imprimeItinerario(solucoes[ind_solucoes].lista_itinerarios[indice_cam]);
                                            indice_cam++;
                                        }
                                    }
                                    printf("\n");
                                    printf("Custo total até o momento: %d\n", solucoes[ind_solucoes].custo_total);
                                    printf("Num de caminhões até o momento: %d\n", solucoes[ind_solucoes].num_caminhoes);
                                    solucoes[ind_solucoes].custo_final = solucoes[ind_solucoes].custo_total + solucoes[ind_solucoes].num_caminhoes;
                                    printf("Custo final: %d\n", solucoes[ind_solucoes].custo_final);
                                    ind_solucoes++;
                                }
                                for(p=0; p < tam_frota_3P; p++)
                                {
                                    if(combinacoesIti3P[z][p] != -1)
                                    {
                                        struct itinerario iti_temp = buscaItinerario(combinacoesIti3P[z][p],
                                                                        lista_itinerarios_3P,NUM_ITI_3P);
                                        resetCidadesEscolhidas(iti_temp.cidades_iti,cidades_escolhidas,
                                                                   iti_temp.num_cidades);
                                    }
                                }

                            } ///end else combinacoesIti3P
                        }///end z
                    }///end k

                    for(p=0; p < tam_frota_5P; p++)
                    {
                        if(combinacoesIti5P[m][p] != -1)
                        {
                            struct itinerario iti_temp = buscaItinerario(combinacoesIti5P[m][p],
                                                        lista_itinerarios_5P,NUM_ITI_5P);
                            resetCidadesEscolhidas(iti_temp.cidades_iti,cidades_escolhidas,
                                        iti_temp.num_cidades);
                        }
                    }
                }///end else combinacoesIti5P
            }///end m
        }///end j
    }///end i

    int ult_sol;
    for(i=0; i < total_escolhas; i++)
    {
        if(solucoes[i].custo_total == 0)
        {
            solucoes[i].custo_total = INT_MAX;
            ult_sol = i;
            break;
        }
    }

    printf("Posição da última solução válida: %d\n",ult_sol);

    quick_solucoes(solucoes,0,ult_sol-1);

    printf("Solução de menor custo:\n");
    printf("Custo total até o momento: %d\n", solucoes[0].custo_total);
    printf("Num de caminhões até o momento: %d\n", solucoes[0].num_caminhoes);
    printf("Itinerários: ");
    for(i=0; i < solucoes[0].num_caminhoes; i++)
        for(j=0; j < solucoes[0].lista_itinerarios[i].num_cidades; j++)
            printf("%d ", solucoes[0].lista_itinerarios[i].cidades_iti[j]);

    printf("\n");
    FILE *arq = fopen("forcabruta.txt","w");

    for(k=0; k < ult_sol; k++)
    {
        if(solucoes[k].custo_total != INT_MAX)
        {
            fprintf(arq,"\nInformações da solução %d\n", k);
            fprintf(arq,"Custo total: %d\n", solucoes[k].custo_total);
            fprintf(arq,"Num de caminhões: %d\n", solucoes[k].num_caminhoes);
            fprintf(arq,"Itinerários: ");
            for(i=0; i < solucoes[k].num_caminhoes; i++)
            {
                if(i == 0)
                    fprintf(arq,"Itinerário do caminhão 10P: ");
                else if (i >= 1 && i <= 4)
                    fprintf(arq,"Itinerário do caminhão 5P: ");
                else
                    fprintf(arq,"Itinerário do caminhão 3P: ");
                for(j=0; j < solucoes[k].lista_itinerarios[i].num_cidades; j++)
                {

                    fprintf(arq,"%c ", cidades[solucoes[k].lista_itinerarios[i].cidades_iti[j]]);
                }
                fprintf(arq,"\n");
            }
        }
    }
    fclose(arq);

    for(i=0; i < MAX; i++)
    {
        free(lista_itinerarios_10P[i].cidades_iti);
        free(lista_itinerarios_5P[i].cidades_iti);
        free(lista_itinerarios_3P[i].cidades_iti);
    }
    free(lista_itinerarios_10P);
    free(lista_itinerarios_5P);
    free(lista_itinerarios_3P);

    for(i=0; i < total_escolhas; i++)
    {
        free(solucoes[i].lista_itinerarios);
    }
    free(solucoes);

    return 0;
}
