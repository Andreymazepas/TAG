/*
Projeto 2 de Teoria e Aplicacao de Grafos 2018/2
Andrey Emmanuel Matrosov Mazépas - 16/0112362 

*/

#include <stdio.h>
#include <stdlib.h>
#include "readgml/readgml.h"
#include "graphlib/graph.h" // biblioteca auxiliar de grafo
// implementacao simples de uma pilha
#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))

NETWORK *mynetwork; // estrutura onde será guardado o grafo
FILE *myfile; // arquivo GML
int stack[100]; // espaco em memoria para a pilha global
int *sp; // ponteiro da pilha
int NVERTICES;

void printNetwork(){
      for(int i=1; i< mynetwork->nvertices+1; i++){
        printf("Disciplina %s:\n", mynetwork->vertex[i-1].label);
        if(mynetwork->vertex[i-1].degree == 0)
            printf("\n");
        else
            printf("\t");
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


void topologicalRecursive(int v_id, int visited[]){
    visited[v_id] = 1; // marca o atual como visitado
    VERTEX v = mynetwork->vertex[v_id];

    for(int i = 0; i < v.degree; i++){
        VERTEX adj  = mynetwork->vertex[v.edge[i].target]; // vertice adjacente ao atual
        if (!visited[adj.id]){
            topologicalRecursive(adj.id, visited);
        }
    }

    push(sp, v_id);
}

void printTopologicalOrder(){

    sp = stack; // inicializa a pilha
    int visited[NVERTICES];
    
    for(int i = 0; i < NVERTICES; i++)
        visited[i] = 0;
    

    for(int i=0; i< NVERTICES; i++){
        VERTEX v = mynetwork->vertex[i];
        if(visited[v.id] == 0){
            topologicalRecursive(v.id, visited);
        }
    }

    while(sp != stack){
        printf("%s\n", mynetwork->vertex[pop(sp)].label);
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

    NVERTICES = mynetwork->nvertices;

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
        printTopologicalOrder();
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