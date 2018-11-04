#include <stdio.h>
#include <stdlib.h>
#include <ngraph.hpp>
#define DG_DYNARR_IMPLEMENTATION
#include "DG_dynarr.h" // biblioteca auxiliar de arrays dinamicos
DA_TYPEDEF(int, intArray);
#define PROFESSORES_OFFSET -1
#define ESCOLAS_OFFSET 99
#define NUMERO_PROFESSORES 100
#define NUMERO_ESCOLAS 50

struct Escola {
    int id;
    int vagas;
    int habilitacao;
    int vagasOcupadas;
    int professores[3];
};

struct Professor {
    int id;
    int habilitacao;
    int escola;
    int preferencias[5];
};


//globais
int professoresOcupados = 0;
int escolasComProf = 0;
int main(void) {

    using namespace NGraph;
    Graph Grafo;
    intArray habilitacoes;
    da_init(habilitacoes);
    da_addn_uninit(habilitacoes, NUMERO_ESCOLAS + NUMERO_PROFESSORES); // array pra manter as preferencias de habilitacao das escolas
                                                                // e habilitacoes dos professores

    FILE * fp;
    fp = fopen("data/entradaProj3TAG.txt", "r");
    if (fp == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }

    Professor Professores[100];
    Escola Escolas[50];

    for(int i=0; i<NUMERO_PROFESSORES; i++){
        int professor, hab_professor, e1, e2, e3, e4, e5;
        fscanf(fp, "(P%d,%d): (E%d, E%d, E%d, E%d, E%d)\n", &professor, &hab_professor, &e1, &e2, &e3, &e4, &e5);
        da_set(habilitacoes, professor + PROFESSORES_OFFSET, hab_professor); // guarda a habilitacao do professor no array
        
        Professores[i].escola = 0;
        Professores[i].id = professor;
        Professores[i].habilitacao = hab_professor;
        Professores[i].preferencias[0] = e1;
        Professores[i].preferencias[1] = e2;
        Professores[i].preferencias[2] = e3;
        Professores[i].preferencias[3] = e4;
        Professores[i].preferencias[4] = e5;
        Professores[i].escola = 0;


        // cria arestas entre o professor e as escolas
        Grafo.insert_edge(professor + PROFESSORES_OFFSET, e1 + ESCOLAS_OFFSET);
        Grafo.insert_edge(professor + PROFESSORES_OFFSET, e2 + ESCOLAS_OFFSET);
        Grafo.insert_edge(professor + PROFESSORES_OFFSET, e3 + ESCOLAS_OFFSET);
        Grafo.insert_edge(professor + PROFESSORES_OFFSET, e4 + ESCOLAS_OFFSET);
        Grafo.insert_edge(professor + PROFESSORES_OFFSET, e5 + ESCOLAS_OFFSET);
    }

    for(int i=0; i<NUMERO_ESCOLAS; i++){
        int escola, habilitacao, vagas;
        fscanf(fp, "(E%d):(%d):(%d)\n",&escola, &habilitacao, &vagas);
        da_set(habilitacoes, escola + ESCOLAS_OFFSET, habilitacao);

        Escolas[i].id = escola;
        Escolas[i].habilitacao = habilitacao;
        Escolas[i].vagas = vagas;
        Escolas[i].vagasOcupadas = 0;

        // percorre todos os professores do array de habilitacoes
        // criando arestas entre a escola e professores
        for(int j = 0; j < ESCOLAS_OFFSET; j++)
        {
            if(da_get(habilitacoes, j) >= habilitacao){
                Grafo.insert_edge(escola+ESCOLAS_OFFSET, j);
            }
        }
        
    }
    std::cout << Grafo;

    
    return 0;
}