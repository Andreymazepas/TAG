#include <stdio.h>
#include <stdlib.h>
#include "readgml/readgml.h"
#include "graphlib/graph.h"
#define DG_DYNARR_IMPLEMENTATION
#include "DG_dynarr.h"

NETWORK *mynetwork;
FILE *myfile;
DA_TYPEDEF(int, intArray);
void BronKerbosch(intArray, intArray, intArray);
int findIDX(intArray arr, int n);
intArray intersectionIntArr(intArray arr1, intArray arr2);
void printIntArr(intArray* arr, const char* name);
intArray Vizinhos(int v);

//    BronKerbosch1(R, P, X):
//        if P and X are both empty:
//            report R as a maximal clique
//        for each vertex v in P:
//            BronKerbosch1(R ⋃ {v}, P ⋂ N(v), X ⋂ N(v))
//            P := P \ {v}
//            X := X ⋃ {v}

void BronKerbosch(intArray R, intArray P, intArray X){
    // printf("Bronkerbusch:\n");
    // printIntArr(&R, "R");
    // printIntArr(&P, "P");
    // printIntArr(&X, "X");
    
    if(da_count(P) + da_count(X) == 0){
        printIntArr(&R, "vertices");
    }
    intArray P1;
    da_init(P1);
    
    for(int i = 0; i < da_count(P); i++)
        da_add(P1, da_get(P,i));
    
    for(int i =0 ; i < da_count(P); i++)
    {
        int v = da_get(P, i);
        da_add(R, v);
        // intArray memama;
        // da_init(memama);
        // memama = Vizinhos(v);
        // intArray mechupa;
        // da_init(mechupa);
        // mechupa = intersectionIntArr(P1, Vizinhos(v));
        // printIntArr(&memama, "vizinhos");
        // printIntArr(&mechupa, "intersection");
        BronKerbosch(R, intersectionIntArr(P1, Vizinhos(v)),intersectionIntArr(X, Vizinhos(v)));
        da_delete(R, findIDX(R, v));
        da_delete(P1, findIDX(P1, v));
        da_add(X, v);
    }
    

}

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

void printIntArr(intArray* arr, const char* name)
{
    // note that arr is a pointer here, so use *arr in the da_*() functions.
    printf("%s = {", name);
    if(da_count(*arr) > 0)
        printf(" %d", arr->p[0]+1);
    for(int i=1; i<da_count(*arr); ++i)
        printf(", %d", arr->p[i]+1);
    printf(" }\n");
}


int main() {

    // faz a leitura do arquivo GML para a estrutura NETWORK
    mynetwork = (NETWORK *)malloc(sizeof(NETWORK));    
    myfile = fopen("data/gmldata/karate.gml", "r");
    if (myfile == NULL){
        printf("Error opening file");
        return -1;
    }
    read_network(mynetwork, myfile);
    fclose(myfile);

    // inicia um array com todos os vertices

    

    // (1) o vértice, e seu respectivo grau (para todos os vértices);
    printf("- Possui %d vértices -\n", mynetwork->nvertices);

    intArray P;
    da_init(P);
    for(int i=1; i< mynetwork->nvertices+1; i++){
        //printf("vertex %d: %d\n", i, mynetwork->vertex[i-1].degree);
        da_add(P, mynetwork->vertex[i-1].id - 1);
    }
    intArray teste = Vizinhos(33);
    printIntArr(&teste, "teste");

    intArray R, X;
    da_init(R);
    da_init(X);
    BronKerbosch(R, P, X);
    
    // (2) todos os cliques maximais (indicando o número de vértices e quais);
    // (3) O Coeficiente de Aglomeração de cada vértice;
    // (4) O Coeficiente médio de Aglomeração do Grafo.
    return 0;
}

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

int findIDX(intArray arr, int n){
    for(int i =0; i<da_count(arr); i++) {
        if (n == da_get(arr, i))
            return i;
    }
    return -1;
}