CC = cc
CFLAGS = -std=c99 -Wall
OBJECTS = randmst.o

all: randmst

randmst.o: randmst.c
	$(CC) $(CFLAGS) -c randmst.c

randmst: $(OBJECTS)
	$(CC) $(OBJECTS) -o randmst -lm

clean:
	rm -f *.o randmst