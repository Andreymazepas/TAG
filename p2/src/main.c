/*
Projeto 2 de Teoria e Aplicacao de Grafos 2018/2
Andrey Emmanuel Matrosov Mazépas - 16/0112362 

*/

#include <stdio.h>
#include <stdlib.h>
#include "readgml/readgml.h"
#include "graphlib/graph.h" // biblioteca auxiliar de grafo

NETWORK *mynetwork; // estrutura onde será guardado o grafo
FILE *myfile; // arquivo GML

int main() {
    // faz a leitura do arquivo GML para a estrutura NETWORK
    mynetwork = (NETWORK *)malloc(sizeof(NETWORK));    
    myfile = fopen("data/Untitled.gml", "r");
    if (myfile == NULL){
        printf("Error opening file\n");
        return -1;
    }
    read_network(mynetwork, myfile);
    fclose(myfile);

    for(int i=1; i< mynetwork->nvertices+1; i++){
        printf("vertex %d: grau %d label %s weight %d\n", i, mynetwork->vertex[i-1].degree, mynetwork->vertex[i-1].label, mynetwork->vertex[i-1].weight);
    }
}