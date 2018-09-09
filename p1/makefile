CC=gcc
ODIR = obj
SDIR = src
INC = -Isrc -Ilib
CFLAGS= -Wall -std=c99
main: readgml.o graphlib.o
	$(CC) $(INC) -g src/main.c obj/readgml.o -o p1 $(CLAGS)
readgml.o:
	$(CC) -c $(INC) lib/readgml/readgml.c -o readgml.o $(CFLAGS)
	mv *.o $(ODIR) 
graphlib.o:
	$(CC) -c $(INC) lib/graphlib/graph.c -o graphlib.o 
	mv *.o $(ODIR) 