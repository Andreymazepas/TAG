#include <stdio.h>
#include <stdlib.h>
#include <ngraph.hpp>
#define DG_DYNARR_IMPLEMENTATION
#include "DG_dynarr.h" // biblioteca auxiliar de arrays dinamicos
DA_TYPEDEF(int, intArray);
#define NUMERO_VERTICES 100
#define NUMERO_ARESTAS 25571

int main(void) {

    using namespace NGraph;
    Graph Grafo;

    FILE * fp;
    fp = fopen("data/email-Eu-core.txt", "r");
    if (fp == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }


    for(int i=0; i<NUMERO_ARESTAS; i++){
        int aresta1, aresta2;
        fscanf(fp, "%d %d\n", &aresta1, &aresta2);        
        Grafo.insert_edge(aresta1, aresta2);
    }

    std::cout << Grafo;

    
    return 0;
}