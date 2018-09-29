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


void printNetwork(){
      for(int i=1; i< mynetwork->nvertices+1; i++){
        printf("vertex %d: label %s weight %d\n", i, mynetwork->vertex[i-1].label, mynetwork->vertex[i-1].weight);
        if(mynetwork->vertex[i-1].degree == 0)
            printf("\n");
        for(int j = 0; j < mynetwork->vertex[i-1].degree; j++)
        {
            int target = mynetwork->vertex[i-1].edge[j].target;
            printf("%s", mynetwork->vertex[target].label);
            if(j == mynetwork->vertex[i-1].degree-1){
                printf("\n\n");}
            else {
                printf("; ");}
        }
        
    }
}
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

    int choice=0;
    while(choice!='4')
    {
        printf("\n\tFluxo do curso: Ciencias da Computacao");
        printf("\n\t------------------------------");
        printf("\n\n\t 1. Imprimir DAG");
        printf("\n\t 2. Imprimir Ordenacao Topologica");
        printf("\n\t 3. Imprimir Caminho Critico");
        printf("\n\t 4. Sair");
        printf("\n\n Entre com a opcao: ");
        choice = getchar();
        switch(choice)
        {
        case '1':
        printf("\n\n");
        printNetwork();
        printf("Pressione uma tecla para voltar ao menu\n");
        (void)getchar();
        break;
        case '2':
        printf("\n\n");
        break;
        case '3':
        printf("\n\n");
        break;
        case '4':
        printf("\n\n");
        break;
        default:
        printf("\n\nOpcao invalida...Tente novamente\n");
        }
        (void)getchar();
    }
    return 0;
}