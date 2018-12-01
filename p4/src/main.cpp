#include <stdio.h>
#include <stdlib.h>
#include <ngraph.hpp>
#define DG_DYNARR_IMPLEMENTATION
#include "DG_dynarr.h" // biblioteca auxiliar de arrays dinamicos
DA_TYPEDEF(int, intArray);
#define NUMERO_VERTICES 1005
#define NUMERO_ARESTAS 25571
using namespace NGraph;
int num_edges = 0;

double getModularidade(Graph Grafo, Graph::vertex_set S)
{
    double arestasAtotal = 0.0;
    double arestasEtotal = 0.0;

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
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

    std::vector<std::vector<uint>> comunidades;

    for (std::vector<uint>::const_iterator i = vertices.begin(); i != vertices.end(); ++i)
    {
        std::vector<uint> aux;
        aux.push_back(*i);
        comunidades.push_back(aux);
    }
    num_edges = Grafo.num_edges();
    int nArestas = comunidades.size();
    int totalComunidades = 0;
    int iterador = 0;

    bool atualizouComunidade = true;
    while (atualizouComunidade)
    {
        atualizouComunidade = false;
        Graph::vertex_set possivelComunidade;
        Graph::vertex_set comunidadeAtual;
        std::cout << "calculando somatorio de modularidades..." << std::endl;
        double somatorioModularidadeAtual = getSomatorioModularidade(Grafo, comunidades);
        double melhorModularidade = somatorioModularidadeAtual;
        int indiceMelhorModularidade = -1;
        std::cout << melhorModularidade << std::endl;
        int par1 = -1;
        int par2 = -1;

        // a ideia é percorrer todos as outras comunidades e combinar com a atual pra ver quem tem a melhor modularidade
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

        for (int z = 0; z < comunidades.size(); z++)
        {
            std::cout << z << " de " << comunidades.size() << std::endl;

            comunidadeAtual.clear();
            for (std::vector<uint>::const_iterator j = comunidades.at(z).begin(); j != comunidades.at(z).end(); ++j)
                comunidadeAtual.insert(*j);
            double modularidadeAtual = modularidadesPrecalculadas.at(z);

            for (int i = z + 1; i < comunidades.size(); i++)
            {
                possivelComunidade.clear();
                for (std::vector<uint>::const_iterator j = comunidades.at(i).begin(); j != comunidades.at(i).end(); ++j)
                {
                    possivelComunidade.insert(*j);
                }
                double modularidadeTotal = somatorioModularidadeAtual - (modularidadesPrecalculadas.at(i) + modularidadeAtual);
                for (Graph::vertex_set::const_iterator t = comunidadeAtual.begin(); t != comunidadeAtual.end(); t++)
                    possivelComunidade.insert(*t);
                double possivelModularidade = modularidadeTotal + getModularidade(Grafo, possivelComunidade);

                if ((possivelModularidade >= melhorModularidade))
                {
                    melhorModularidade = possivelModularidade;
                    par1 = z;
                    par2 = i;
                    atualizouComunidade = true;
                }
            }
        }

        if (atualizouComunidade)
        {
            comunidades.at(par1).insert(comunidades.at(par1).end(), comunidades.at(par2).begin(), comunidades.at(par2).end());
            comunidades.erase(comunidades.begin() + par2);
            iterador++;
            totalComunidades++;
        }
    }
    std::cout << "TOTAL COMUNIDADES: " << comunidades.size() << std::endl;

    for (int i = 0; i < comunidades.size(); i++)
    {
        std::cout << i << ": ";
        for (std::vector<uint>::const_iterator j = comunidades.at(i).begin(); j != comunidades.at(i).end(); ++j)
            std::cout << *j << ' ';
        std::cout << std::endl;
    }

    return 0;
}