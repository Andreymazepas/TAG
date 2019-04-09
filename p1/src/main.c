/*
Projeto 1 de Teoria e Aplicacao de Grafos 2018/2
Andrey Emmanuel Matrosov Mazépas - 16/0112362
Kevin da Silva Souto - 16/0046688

*/

#include <stdio.h>
#include <stdlib.h>
#include "readgml/readgml.h"
#include "graphlib/graph.h" // biblioteca auxiliar de grafo
#define DG_DYNARR_IMPLEMENTATION
#include "DG_dynarr.h" // biblioteca auxiliar de arrays dinamicos





// ********************** declaracoes iniciais ***********************

NETWORK *mynetwork; // estrutura onde será guardado o grafo
FILE *myfile; // arquivo GML
DA_TYPEDEF(int, intArray);
void BronKerbosch(intArray, intArray, intArray);
int findIDX(intArray arr, int n);
intArray intersectionIntArr(intArray arr1, intArray arr2);
void printIntArr(intArray* arr, const char* name);
intArray Vizinhos(int v);
double aglomeracao(VERTEX v);
int isVizinho(int, int);
double mediaAglomeracao();

// *********************************************************************



int main() {
    // faz a leitura do arquivo GML para a estrutura NETWORK
    mynetwork = (NETWORK *)malloc(sizeof(NETWORK));
    myfile = fopen("data/gmldata/karate.gml", "r");
    if (myfile == NULL){
        printf("Error opening file\n");
        return -1;
    }
    read_network(mynetwork, myfile);
    fclose(myfile);


    // (1) o vértice, e seu respectivo grau (para todos os vértices);
    printf("*** Grafo possui %d vértices ***\n", mynetwork->nvertices);
    printf("(1) o vértice, e seu respectivo grau (para todos os vértices)\n");
    // inicia um array com todos os vertices e printa seu grau
    intArray P;
    da_init(P);
    for(int i=1; i< mynetwork->nvertices+1; i++){
        printf("vertex %d: grau %d\n", i, mynetwork->vertex[i-1].degree);
        da_add(P, mynetwork->vertex[i-1].id - 1);
    }


    // (2) todos os cliques maximais (indicando o número de vértices e quais);
    printf("\n\n(2) todos os cliques maximais (indicando o número de vértices e quais)\n");
    // inicia dois arrays vazios para o algoritmo
    intArray R, X;
    da_init(R);
    da_init(X);
    BronKerbosch(R, P, X); // algoritmo de BronKerbosch


    // (3) O Coeficiente de Aglomeração de cada vértice;
    printf("\n\n(3) O Coeficiente de Aglomeração de cada vértice\n");
    for(int i=0; i < mynetwork->nvertices; i++){
        printf("vertice %d: %f\n", i+1, aglomeracao(mynetwork->vertex[i]));
    }


    // (4) O Coeficiente médio de Aglomeração do Grafo.
    printf("\n\n(4) O Coeficiente médio de Aglomeração do Grafo.\n");
    printf("Coeficiente médio de aglomeração: %f\n", mediaAglomeracao());

    return 0;
}



//****************************BronKerbosch***************************

// pseudocodigo:
//    BronKerbosch1(R, P, X):
//        if P and X are both empty:
//            report R as a maximal clique
//        for each vertex v in P:
//            BronKerbosch1(R ⋃ {v}, P ⋂ N(v), X ⋂ N(v))
//            P := P \ {v}
//            X := X ⋃ {v}

void BronKerbosch(intArray R, intArray P, intArray X){
//        if P and X are both empty:
    if(da_count(P) + da_count(X) == 0){
        printf("%ld ", da_count(R));
        printIntArr(&R, "vertices"); //    report R as a maximal clique
    }

    intArray P1; // faz uma copia de P
    da_init(P1);
    for(int i = 0; i < da_count(P); i++)
        da_add(P1, da_get(P,i));

    //        for each vertex v in P:
    for(int i =0 ; i < da_count(P); i++)
    {
        int v = da_get(P, i);
        da_add(R, v); // R ⋃ {v}
        BronKerbosch(R, intersectionIntArr(P1, Vizinhos(v)),intersectionIntArr(X, Vizinhos(v))); //   BronKerbosch1(R ⋃ {v}, P ⋂ N(v), X ⋂ N(v))
        da_delete(R, findIDX(R, v)); // reseta o R
        da_delete(P1, findIDX(P1, v)); //   P := P \ {v}
        da_add(X, v);  //            X := X ⋃ {v}
    }
}
//*************************************************************************


double aglomeracao(VERTEX v){
    int grau = v.degree;
    intArray vizinhos;
    da_init(vizinhos);
    vizinhos = Vizinhos(v.id -1);
    int links = 0;

    // encontrar o numero de links entre os vizinhos
    for(int i = 0; i < da_count(vizinhos); i++)
    {
        for(int j = i+1; j < da_count(vizinhos); j++)
        {
            if(i != j && isVizinho(da_get(vizinhos,i)-1, da_get(vizinhos,j)-1))
                links++;
        }
    }
    //printf("id = %d ,links = %d, grau = %d\n", v.id,links, grau);
    //printIntArr(&vizinhos, "vizinhos");
    double coefaglomeracao = (2.0*links) / (grau*(1.0*(grau-1)));
    if(links == 0)
        coefaglomeracao = 0;
    return coefaglomeracao;
}

double mediaAglomeracao(){
    int nVertices = mynetwork->nvertices;
    double mediaAglomeracao_ = 0;

    for(int i = 0; i < nVertices; i++)
    {
        mediaAglomeracao_ += aglomeracao(mynetwork->vertex[i]);
    }
    mediaAglomeracao_ = (1.0 / nVertices) * mediaAglomeracao_;
    return mediaAglomeracao_;

}



//******************** funcoes auxiliares **********************************

// retorna a interseccao de dois arrays
intArray intersectionIntArr(intArray arr1, intArray arr2){
    intArray intersection;
    da_init(intersection);
    for(int i = 0; i < da_count(arr1); i++)
    {
        for(int j =0; j<da_count(arr2); j++){
            if(da_get(arr1, i) == da_get(arr2, j)){
                da_add(intersection, da_get(arr1, i));
                break;
            }
        }
    }
    return intersection;
}


// printa um array
void printIntArr(intArray* arr, const char* name)
{
    printf("%s = {", name);
    if(da_count(*arr) > 0)
        printf(" %d", arr->p[0]+1);
    for(int i=1; i<da_count(*arr); ++i)
        printf(", %d", arr->p[i]+1);
    printf(" }\n");
}



// retorna um array com todos os vizinhos de um vertice V
intArray Vizinhos(int v){
    intArray vizinhos;
    da_init(vizinhos);
    EDGE *edge = mynetwork->vertex[v].edge;
    for(int j=0; j < mynetwork->vertex[v].degree; j++)
    {
        da_add(vizinhos, (edge->target));
        edge++;
    }
    return vizinhos;
}

// checa se v1 e v2 são vizinhos
int isVizinho(int v1, int v2){
    intArray v1Vizinhos = Vizinhos(v1);

    for(int i = 0; i < da_count(v1Vizinhos); i++)
    {
        if(da_get(v1Vizinhos, i) == v2)
            return 1;
    }
    return 0;
}


// retorna o indice de um item n em um array
int findIDX(intArray arr, int n){
    for(int i =0; i<da_count(arr); i++) {
        if (n == da_get(arr, i))
            return i;
    }
    return -1;
}
//***************************************************************************
