#include <stdio.h>
#include <stdlib.h>
#include <ngraph.hpp>
#define DG_DYNARR_IMPLEMENTATION
#include "DG_dynarr.h" // biblioteca auxiliar de arrays dinamicos
DA_TYPEDEF(int, intArray);
#define NUMERO_VERTICES  34 //100
#define NUMERO_ARESTAS 78 // 25571
using namespace NGraph;


double getModularidade(Graph Grafo, Graph::vertex_set S) {
    double arestasAtotal = 0.0;
    double arestasEtotal = 0.0;

    for (Graph::vertex_set::const_iterator t = S.begin(); t !=S.end(); t++)
        {
//            std::cout << "loop vertice da comunidade: " << *t << std::endl;
            double arestasEntrando = 0.0;
            double arestasEsperadasEntrando = 0.0;
            arestasEsperadasEntrando = Grafo.out_degree(*t);
            arestasAtotal += Grafo.out_degree(*t);
            
            Graph::vertex_set S2 = Grafo.out_neighbors(*t);
            for(Graph::vertex_set::const_iterator vertex = Grafo.in_neighbors(*t).begin(); vertex != Grafo.in_neighbors(*t).end(); vertex++)
            {
//                std::cout << "analisando vizinho " << *vertex << std::endl;
                for (Graph::vertex_set::const_iterator t2 = S.begin(); t2 !=S.end(); t2++){
                    if(*t2 == *vertex){
//                        std::cout << *t2 << " é igual a " << *vertex << std::endl;
                        arestasEntrando++;
                    }
                }

            }
            arestasEtotal += arestasEntrando;
//            std::cout << "vertex" << *t << std::endl;
//            std::cout << "arestas esperadas: " << arestasEsperadasEntrando << std::endl;
//            std::cout << "arestas entrando: " << arestasEntrando << std::endl;
            
        }

//   std::cout << Grafo;

//    std::cout << "arestasAtotal: " << arestasAtotal << " arestasEtotal: " << arestasEtotal << std::endl;
//    std::cout << "num_edges: " << Grafo.num_edges() << std::endl;
    arestasAtotal /= Grafo.num_edges();
    arestasAtotal = arestasAtotal * arestasAtotal;
    arestasEtotal /= Grafo.num_edges();
//    std::cout << (arestasEtotal - arestasAtotal) << std::endl;
    return (arestasEtotal - arestasAtotal);
}

int main(void) {

    Graph Grafo;

    FILE * fp;
    fp = fopen("data/teste.txt", "r");
    if (fp == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 0;
    }

    std::vector<uint> vertices;
    for(int i=0; i<NUMERO_ARESTAS; i++){
        int aresta1, aresta2;
        fscanf(fp, "%d %d\n", &aresta1, &aresta2);        
        Grafo.insert_edge(aresta1, aresta2);
        Grafo.insert_edge(aresta2, aresta1); 
        vertices.push_back(aresta1);
        vertices.push_back(aresta2);
    }
    for (std::vector<uint>::const_iterator i = vertices.begin(); i != vertices.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    std::sort( vertices.begin(), vertices.end() );
    vertices.erase( std::unique( vertices.begin(), vertices.end() ), vertices.end() );
    

    for (std::vector<uint>::const_iterator i = vertices.begin(); i != vertices.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    
    int nArestas = NUMERO_ARESTAS;
    Graph::vertex a = 6;
    Graph::vertex_set S;
    S.insert(1);
    S.insert(12);
    S.insert(13);
    S.insert(5);
    S.insert(6);
    S.insert(7);


    Graph::vertex_set S1;
    S1.insert(2);
    S1.insert(3);
    S1.insert(4);
    S1.insert(8);
    S1.insert(9);
    S1.insert(10);
    S1.insert(11);

    std::cout << getModularidade(Grafo, S1) + getModularidade(Grafo, S) << std::endl; 

    S.clear();
    double maxModularidade = -50.0;
    int maxModularidadeId = 0;
    Graph::vertex begin;
      for ( Graph::const_iterator p = Grafo.begin(); p != Grafo.end(); p++) {
          if(p == Grafo.begin()){
              //S.insert(p.begin);
          }
       }

    
    /////////////////////////////////////////////////
// Implementation
// template<typename PGraph>
// double GetModularity(const PGraph& Graph, const TIntV& NIdV, int GEdges) {
//   if (GEdges == -1) { GEdges = Graph->GetEdges(); }
//   double EdgesIn = 0.0, EEdgesIn = 0.0; // EdgesIn=2*number of edges inside the cluster, EEdgesIn=expected edges inside
//   TIntSet NIdSet(NIdV.Len());
//   for (int e = 0; e < NIdV.Len(); e++) { // edges inside
//     NIdSet.AddKey(NIdV[e]);
//   }
//   for (int e1 = 0; e1 < NIdV.Len(); e1++) {
//     typename PGraph::TObj::TNodeI NI = Graph->GetNI(NIdV[e1]);
//     EEdgesIn += NI.GetOutDeg();
//     for (int i = 0; i < NI.GetOutDeg(); i++) {
//       if (NIdSet.IsKey(NI.GetOutNId(i))) { EdgesIn += 1; }
//     }
//   }
//   EEdgesIn = EEdgesIn*EEdgesIn / (2.0*GEdges);
//   if ((EdgesIn - EEdgesIn) == 0) { return 0; }
//   else { return (EdgesIn - EEdgesIn) / (2.0*GEdges); } // modularity
// }
    //std::cout << Grafo;

    
    return 0;
}