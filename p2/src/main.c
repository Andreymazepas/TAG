/*
Projeto 2 de Teoria e Aplicacao de Grafos 2018/2
Andrey Emmanuel Matrosov Mazépas - 16/0112362
Kevin da Silva Souto - 16/0046688

*/

#include <stdio.h>
#include <stdlib.h>
#include "readgml/readgml.h"
#include "graphlib/graph.h" // biblioteca auxiliar de grafo
#define DG_DYNARR_IMPLEMENTATION
#include "DG_dynarr.h" // biblioteca auxiliar de arrays dinamicos
// implementacao simples de uma pilha
#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))

// struct definindo um caminho com origem, destino e um peso atribuido
struct path
{
    int origem;
    int destino;
    int peso;
};


DA_TYPEDEF(int, intArray); //definindo tipo intarray
DA_TYPEDEF(struct path, pathArray);
void printIntArr(intArray* arr, const char* name);
NETWORK *mynetwork; // estrutura onde será guardado o grafo
FILE *myfile; // arquivo GML
int stack[100]; // espaco em memoria para a pilha global
int *sp; // ponteiro da pilha
int NVERTICES; // numero maximo de vertices


// funcao para imprimir toda o grafo
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


//funcao simples para encontrar todos os vertices de origem do grafo
intArray encontraOrigens(){

    int arestasEntrada[NVERTICES];
    for(int i=0; i<NVERTICES; i++){
        arestasEntrada[i] =0;
    }

    for(int i = 0; i < NVERTICES; i++)
    {
        VERTEX v = mynetwork->vertex[i];
        for(int j = 0; j < v.degree; j++)
        {
            EDGE e = v.edge[j];
            arestasEntrada[e.target] = arestasEntrada[e.target] + 1;
        }
    }

    intArray idsOrigem;
    da_init(idsOrigem);

    for(int i = 0; i < NVERTICES; i++)
    {
        if(arestasEntrada[i]==0)
            da_add(idsOrigem, i);
    }

    return idsOrigem;
}


// simples DFS para encontrar folhas de um certo no
void DFS(int v_id, int visited[], int peso){
    visited[v_id] = 1; // marca o atual como visitado
    VERTEX v = mynetwork->vertex[v_id];
    peso += v.weight;

    for(int i = 0; i < v.degree; i++){
        VERTEX adj  = mynetwork->vertex[v.edge[i].target]; // vertice adjacente ao atual
        if (!visited[adj.id]){
            DFS(adj.id, visited, peso);
        }
    }
    if(v.degree == 0){
        push(sp, v_id);
        push(sp, peso);
    }
    return ;
}


// funcao para imprimir um caminho critico baseado nos pesos de cada materia
void printCP(){
    intArray origens = encontraOrigens();
    pathArray maxPaths;
    da_init(maxPaths);
    // itera sobre todos os nos de origem para encontrar as folhas e o caminho
    // mais pesado entre origem -> folha
    for(int i =0; i < da_count(origens); i++){
        sp = stack;
        int visited[NVERTICES];

        for(int i = 0; i < NVERTICES; i++)
            visited[i] = 0;

        intArray folhas;
        da_init(folhas);
        DFS(i, visited, 0); // encontra as folhas

        while(sp != stack){
            da_add(folhas, pop(sp));
        }

        int maxPeso = 0;
        int idmaxPeso = 0;
        // ve qual o caminho de maior peso de origem ate a folha
        for(int i = 0; i < da_count(folhas); i+=2)
        {
            if(da_get(folhas, i) > maxPeso){
                maxPeso = da_get(folhas, i);
                idmaxPeso = da_get(folhas, i+1);
            }
        }
        struct path path1;
        path1.origem = i;
        path1.destino = idmaxPeso;
        path1.peso = maxPeso;
        da_add(maxPaths, path1);

    }
   // ve todos os nos analisados e o peso encontrado
    int maxPeso = 0;
    int idmaxPeso = 0;
    for(int i =0; i < da_count(maxPaths);i++){
        struct path path1 = da_get(maxPaths, i);
        if(path1.peso > maxPeso){
            maxPeso = path1.peso;
            idmaxPeso = i;
        }
    }
    struct path pesoMAX = da_get(maxPaths, idmaxPeso);
    printf("CAMINHO DE MAIOR PESO:\n");
    printf("%s -> %s : PESO %d\n", mynetwork->vertex[pesoMAX.origem].label,  mynetwork->vertex[pesoMAX.destino].label, pesoMAX.peso );
    return;

}

// funcao auxiliar para encontrar um caminho topologico
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

    // itera sobre todos os nos para realizar a ordenacao topologica
    for(int i=0; i< NVERTICES; i++){
        VERTEX v = mynetwork->vertex[i];
        if(visited[v.id] == 0){
            topologicalRecursive(v.id, visited);
        }
    }
    // iomprime o conteudo da stack em ordem
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
        printCP();
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


// funcao auxiliar para imprimir intArray
void printIntArr(intArray* arr, const char* name)
 {
     printf("%s = {", name);
     if(da_count(*arr) > 0)
         printf(" %d", arr->p[0]);
     for(int i=1; i<da_count(*arr); ++i)
         printf(", %d", arr->p[i]);
     printf(" }\n");
 }

