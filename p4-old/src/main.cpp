#include <stdio.h>
#include <stdlib.h>
#include <ngraph.hpp> // bilbioteca auxiliar com ferramentas para grafos
#define NUMERO_VERTICES 1005
#define NUMERO_ARESTAS 25571
using namespace NGraph;

int num_edges = 0; // variavel global para armazenar previamente o numero de arestas

// Função que retorna a modularidade de um conjunto de vertices em relação ao Grafo total
double getModularidade(Graph Grafo, Graph::vertex_set S)
{
    double arestasAtotal = 0.0;
    double arestasEtotal = 0.0;

    // percorre todo o conjunto de vertices e faz o calculo com arestas que estariam entrando num grafo completo
    // e com as arestas que realmente estao entrando na comunidade
    for (Graph::vertex_set::const_iterator t = S.begin(); t != S.end(); t++)
    {
        double arestasEntrando = 0.0;
        double arestasEsperadasEntrando = 0.0;
        arestasEsperadasEntrando = Grafo.out_degree(*t);
        arestasAtotal += arestasEsperadasEntrando;

        Graph::vertex_set S2 = Grafo.out_neighbors(*t);
        for (Graph::vertex_set::const_iterator vertex = Grafo.in_neighbors(*t).begin(); vertex != Grafo.in_neighbors(*t).end(); vertex++)
        {
            for (Graph::vertex_set::const_iterator t2 = S.begin(); t2 != S.end(); t2++)
            {
                if (*t2 == *vertex)
                {
                    arestasEntrando++;
                }
            }
        }
        arestasEtotal += arestasEntrando;
    }

    arestasAtotal /= num_edges;
    arestasAtotal = arestasAtotal * arestasAtotal;
    arestasEtotal /= num_edges;
    return (-arestasAtotal + arestasEtotal);
}

// Função que retorna o somatorio de todas as modularidades (tratado no artigo como Q)
// de um conjunto de comunidades.
double getSomatorioModularidade(Graph Grafo, std::vector<std::vector<uint>> comunidades)
{

    double somatorio;
    Graph::vertex_set aux;
    for (int i = 0; i < comunidades.size(); i++)
    {
        aux.clear();
        for (std::vector<uint>::const_iterator j = comunidades.at(i).begin(); j != comunidades.at(i).end(); ++j)
        {
            aux.insert(*j);
        }
        somatorio += getModularidade(Grafo, aux);
    }

    return somatorio;
}

int main(void)
{

    Graph Grafo;

    // Abre e le o arquivo carregando vertices e arestas nas estruturas de Grafo e comunidades
    FILE *fp;
    fp = fopen("data/karate.txt", "r");
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }

    std::vector<uint> vertices;
    for (int i = 0; i < NUMERO_ARESTAS; i++)
    {
        int aresta1, aresta2;
        fscanf(fp, "[%d %d]\n", &aresta1, &aresta2);
        Grafo.insert_edge(aresta1, aresta2);
        Grafo.insert_edge(aresta2, aresta1);
        vertices.push_back(aresta1);
        vertices.push_back(aresta2);
    }

    std::cout << std::endl;
    std::sort(vertices.begin(), vertices.end());
    // apaga vertices e arestas repetidos por nao se tratar de um digrafo
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

    std::vector<std::vector<uint>> comunidades;

    for (std::vector<uint>::const_iterator i = vertices.begin(); i != vertices.end(); ++i)
    {
        std::vector<uint> aux;
        aux.push_back(*i);
        comunidades.push_back(aux);
    }

    // Inicio do algoritmo
    num_edges = Grafo.num_edges();
    bool atualizouComunidade = true;
    double melhorModularidade;

    // O loop se mantem enquanto tiverem mudanças no conjunto de comunidades
    // uma mudança se dá quando há pelo menos um par que aumenta o Q total do grafo
    while (atualizouComunidade)
    {
        atualizouComunidade = false;
        Graph::vertex_set possivelComunidade; // par a ser analisado
        Graph::vertex_set comunidadeAtual;    // a comunidade sendo comparada

        // calculando somatorio de modularidades antes de aplicar o algoritmo
        double somatorioModularidadeAtual = getSomatorioModularidade(Grafo, comunidades);
        melhorModularidade = somatorioModularidadeAtual; // variavel para guardar o melhor valor de modularidade encontrado
        std::cout << melhorModularidade << std::endl;           // imprime a modularidade encontrada, que sera a total
        int par1 = -1;                                          // primeiro elemento do par que produz a melhor modularidade
        int par2 = -1;                                          // segundo elemento

        // para melhorar a performance e evitar repetir chamadas de funcoes iguais
        // a modularidade individual das comunidades nesse passo serão pre calculadas
        std::vector<double> modularidadesPrecalculadas;
        for (int i = 0; i < comunidades.size(); i++)
        {
            Graph::vertex_set aux;
            for (std::vector<uint>::const_iterator j = comunidades.at(i).begin(); j != comunidades.at(i).end(); ++j)
            {
                aux.insert(*j);
            }
            modularidadesPrecalculadas.push_back(getModularidade(Grafo, aux));
        }

        // a ideia é percorrer todos as comunidades e combinar com a atual pra ver quem tem a melhor modularidade
        // se trata de uma combinacao n dois a dois
        for (int z = 0; z < comunidades.size(); z++)
        {
            comunidadeAtual.clear();
            for (std::vector<uint>::const_iterator j = comunidades.at(z).begin(); j != comunidades.at(z).end(); ++j)
                comunidadeAtual.insert(*j);
            double modularidadeAtual = modularidadesPrecalculadas.at(z); // modularidade da comunidade atual
            // combina com todas as outras
            for (int i = z + 1; i < comunidades.size(); i++)
            {
                possivelComunidade.clear();
                for (std::vector<uint>::const_iterator j = comunidades.at(i).begin(); j != comunidades.at(i).end(); ++j)
                {
                    possivelComunidade.insert(*j);
                }
                // por ser um somatorio, remover duas comunidades e adicionar sua uniao tem o mesmo efeito que recalcular o somatorio com a uniao
                // mais uma tentativa de salvar performance
                double modularidadeTotal = somatorioModularidadeAtual - (modularidadesPrecalculadas.at(i) + modularidadeAtual);
                for (Graph::vertex_set::const_iterator t = comunidadeAtual.begin(); t != comunidadeAtual.end(); t++)
                    possivelComunidade.insert(*t);
                double possivelModularidade = modularidadeTotal + getModularidade(Grafo, possivelComunidade);

                // se o novo par de comunidades encontrado gerar uma modularidade maior que a atual
                // o par sera salvo para atualizar o grafo
                if ((possivelModularidade >= melhorModularidade))
                {
                    melhorModularidade = possivelModularidade;
                    par1 = z;
                    par2 = i;
                    atualizouComunidade = true;
                }
            }
        }
        // se houve mudança nas comunidades, aplicar no vetor comunidades
        // insere a nova uniao e remove a unidade antiga
        // imprime quais foram as comunidades unidas
        if (atualizouComunidade)
        {
            std::cout << std::endl << "{ ";
            for (std::vector<uint>::const_iterator j = comunidades.at(par1).begin(); j != comunidades.at(par1).end(); ++j)
                std::cout << *j << ' ';
            std::cout << "} + { ";
            for (std::vector<uint>::const_iterator j = comunidades.at(par2).begin(); j != comunidades.at(par2).end(); ++j)
                std::cout << *j << ' ';
            std::cout << "}" << std::endl;
            comunidades.at(par1).insert(comunidades.at(par1).end(), comunidades.at(par2).begin(), comunidades.at(par2).end());
            comunidades.erase(comunidades.begin() + par2);
        }
    }

    // ao final do loop imprimir o total de comunidades
    std::cout << "\nTOTAL COMUNIDADES: " << comunidades.size() << std::endl;
    std::cout << "SOMATORIO DE MODULARIDADES DO GRAFO (Q): " << melhorModularidade << std::endl;
    // e quais sao seus elementos
    for (int i = 0; i < comunidades.size(); i++)
    {
        std::cout << i << ": ";
        for (std::vector<uint>::const_iterator j = comunidades.at(i).begin(); j != comunidades.at(i).end(); ++j)
            std::cout << *j << ' ';
        std::cout << std::endl;
    }

    return 0;
}