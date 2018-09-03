#include "readgml.h"
#include <stdio.h>

int main() {
    NETWORK mynetwork;
    FILE *myfile;
    myfile = fopen("gmldata/karate.gml", "r");
    if (myfile == NULL){
        printf("Error opening file");
        return -1;
    }
    int a = read_network(&mynetwork, myfile);
    fclose(myfile);
    printf("a = %d", a);
    return 0;
}