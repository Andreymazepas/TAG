#include <stdio.h>
#include "readgml.h"


int main() {
    NETWORK *mynetwork;
    mynetwork = (NETWORK *)malloc(sizeof(NETWORK));

    FILE *myfile;
    myfile = fopen("lesmis.gml", "r");
    if (myfile == NULL){
        printf("Error opening file");
        return -1;
    }
    int a = read_network(&mynetwork, stdin);
    fclose(myfile);
    printf("a = %d", a);
    return 0;
}