/* Program to do group selection according to the method of Riolo,
 * Cantwell, Reinert, and Newman
 *
 * Written by Mark Newman, January 2017
 */

/* Inclusions */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "readgml.h"

/* Constants */

#define K 25               // Maximum number of groups
#define MCSWEEPS 10000     // Number of Monte Carlo sweeps to perform
#define SAMPLE 1           // Rate at which to print out results, in sweeps

/* Globals */

NETWORK G;            // The network
int twom;             // Twice the number of edges
double p;             // Average edge probability

int k;                // Current value of k
int *g;               // Group assignments
int *n;               // Group sizes
int **m;              // Edge counts
int *kappa;           // Sums of degrees
int **in;             // Lists of the nodes in each group

double *lnfact;       // Look-up table of log-factorials
main(int argc, char *argv[])
{
  int u,r,s;
  double pr,entropy;
  FILE *f;

  // Initialize the random number generator from the system clock


  // Read the network from stdin

  read_network(&G,stdin);
  
}
