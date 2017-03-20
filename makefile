CC = cc
CFLAGS = -std=c99 -Wall
OBJECTS = strassen.o
OBJECTR = randmst.o

all: strassen randmst

strassen.o: strassen.c
	$(CC) $(CFLAGS) -c strassen.c

strassen: $(OBJECTS)
	$(CC) $(OBJECTS) -o strassen -lm

randmat:
	python randmat.py

randmst.o: randmst.c
	$(CC) $(CFLAGS) -c randmst.c

randmst: $(OBJECTR)
	$(CC) $(OBJECTR) -o randmst -lm

clean:
	rm -f *.o strassen randmst
