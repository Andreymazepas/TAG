#include <stdio.h>
#include <stdlib.h>
#include "readgml/readgml.h"
#include "graphlib/graph.h"


int main() {
    NETWORK *mynetwork;
    mynetwork = (NETWORK *)malloc(sizeof(NETWORK));
    FILE *myfile;
    myfile = fopen("data/gmldata/karate.gml", "r");
    if (myfile == NULL){
        printf("Error opening file");
        return -1;
    }
    int a = read_network(mynetwork, myfile);
    fclose(myfile);
    printf("- Possui %d vértices -\n", mynetwork->nvertices);
    for(int i=1; i< mynetwork->nvertices+1; i++){
        printf("vertex %d: %d\n", i, mynetwork->vertex[i-1].degree);
    }
    return 0;
}