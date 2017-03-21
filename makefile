CC = cc
CFLAGS = -std=c99 -Wall
OBJECTS = strassen.o
OBJECTR = randmst.o
OBJECTO = strassen_onepad.o

all: strassen strassen_onepad randmst

test_all: test test_onepad

test:
	./strassen 1 128 random.txt >> results1.txt;
	./strassen 1 256 random.txt >> results1.txt;
	./strassen 1 512 random.txt >> results1.txt;
	./strassen 1 1024 random.txt >> results1.txt;
	./strassen 1 2048 random.txt >> results1.txt;
	./strassen 1 127 random.txt >> results1.txt;
	./strassen 1 255 random.txt >> results1.txt;
	./strassen 1 511 random.txt >> results1.txt;
	./strassen 1 1023 random.txt >> results1.txt;
	./strassen 1 2047 random.txt >> results1.txt;
	./strassen 1 129 random.txt >> results1.txt;
	./strassen 1 257 random.txt >> results1.txt;
	./strassen 1 513 random.txt >> results1.txt;
	./strassen 1 1025 random.txt >> results1.txt;
	./strassen 1 2049 random.txt >> results1.txt;
	./strassen 1 96 random.txt >> results1.txt;
	./strassen 1 192 random.txt >> results1.txt;
	./strassen 1 384 random.txt >> results1.txt;
	./strassen 1 768 random.txt >> results1.txt;
	./strassen 1 1536 random.txt >> results1.txt;

test_onepad:
	./strassen_onepad 1 128 random.txt >> results2.txt;
	./strassen_onepad 1 256 random.txt >> results2.txt;
	./strassen_onepad 1 512 random.txt >> results2.txt;
	./strassen_onepad 1 1024 random.txt >> results2.txt;
	./strassen_onepad 1 2048 random.txt >> results2.txt;
	./strassen_onepad 1 127 random.txt >> results2.txt;
	./strassen_onepad 1 255 random.txt >> results2.txt;
	./strassen_onepad 1 511 random.txt >> results2.txt;
	./strassen_onepad 1 1023 random.txt >> results2.txt;
	./strassen_onepad 1 2047 random.txt >> results2.txt;
	./strassen_onepad 1 129 random.txt >> results2.txt;
	./strassen_onepad 1 257 random.txt >> results2.txt;
	./strassen_onepad 1 513 random.txt >> results2.txt;
	./strassen_onepad 1 1025 random.txt >> results2.txt;
	./strassen_onepad 1 2049 random.txt >> results2.txt;
	./strassen_onepad 1 96 random.txt >> results2.txt;
	./strassen_onepad 1 192 random.txt >> results2.txt;
	./strassen_onepad 1 384 random.txt >> results2.txt;
	./strassen_onepad 1 768 random.txt >> results2.txt;
	./strassen_onepad 1 1536 random.txt >> results2.txt

test_manypad:
	./strassen_manypad 1 128 random.txt >> results3.txt;
	./strassen_manypad 1 256 random.txt >> results3.txt;
	./strassen_manypad 1 512 random.txt >> results3.txt;
	./strassen_manypad 1 1024 random.txt >> results3.txt;
	./strassen_manypad 1 2048 random.txt >> results3.txt;
	./strassen_manypad 1 127 random.txt >> results3.txt;
	./strassen_manypad 1 255 random.txt >> results3.txt;
	./strassen_manypad 1 511 random.txt >> results3.txt;
	./strassen_manypad 1 1023 random.txt >> results3.txt;
	./strassen_manypad 1 2047 random.txt >> results3.txt;
	./strassen_manypad 1 129 random.txt >> results3.txt;
	./strassen_manypad 1 257 random.txt >> results3.txt;
	./strassen_manypad 1 513 random.txt >> results3.txt;
	./strassen_manypad 1 1025 random.txt >> results3.txt;
	./strassen_manypad 1 2049 random.txt >> results3.txt;
	./strassen_manypad 1 96 random.txt >> results3.txt;
	./strassen_manypad 1 192 random.txt >> results3.txt;
	./strassen_manypad 1 384 random.txt >> results3.txt;
	./strassen_manypad 1 768 random.txt >> results3.txt;
	./strassen_manypad 1 1536 random.txt >> results3.txt

strassen.o: strassen.c
	$(CC) $(CFLAGS) -c strassen.c

strassen: $(OBJECTS)
	$(CC) $(OBJECTS) -o strassen -lm

strassen_onepad.o: strassen_onepad.c
	$(CC) $(CFLAGS) -c strassen_onepad.c

strassen_onepad: $(OBJECTO)
	$(CC) $(OBJECTO) -o strassen_onepad -lm

randmat:
	python randmat.py

randmst.o: randmst.c
	$(CC) $(CFLAGS) -c randmst.c

randmst: $(OBJECTR)
	$(CC) $(OBJECTR) -o randmst -lm

clean:
	rm -f *.o strassen randmst
