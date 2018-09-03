CC=gcc
ODIR = obj
SDIR = src
INC = -Isrc -I"lib/gmldata"
CFLAGS= -Wall -std=c99
main: readgml.o
	$(CC) $(INC) src/main.c obj/readgml.o -o p1 $(CLAGS)
readgml.o:
	$(CC) -c $(INC) lib/readgml/readgml.c -o readgml.o $(CFLAGS)
	mv *.o $(ODIR) 