CC = cc
CFLAGS = -std=c99 -Wall
OBJECTS = strassen.o

all: strassen randmst

strassen.o: strassen.c
	$(CC) $(CFLAGS) -c strassen.c

strassen: $(OBJECTS)
	$(CC) $(OBJECTS) -o strassen -lm

randmat:
	$ time (python -c "import random; print('\n'.join('{0}'.format(n) for n in random.sample([0,1]*16*1024*1024, 2*1024*1024)));" > random.txt)

randmst.o: randmst.c
	$(CC) $(CFLAGS) -c randmst.c

randmst: $(OBJECTS)
	$(CC) $(OBJECTS) -o randmst -lm

clean:
	rm -f *.o strassen randmst
