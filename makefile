CC = cc
CFLAGS = -std=c99 -Wall
OBJECTS = strassen_basic.o
OBJECTR = randmst.o
OBJECTO = strassen_onepad.o
OBJECTM = strassen_manypad.o

all: strassen_basic strassen_onepad strassen_manypad randmst

test_all: test_basic test_onepad test_manypad

test_basic:
	./strassen_basic 1 128 random.txt >> results1.txt;
	./strassen_basic 1 256 random.txt >> results1.txt;
	./strassen_basic 1 512 random.txt >> results1.txt;
	./strassen_basic 1 1024 random.txt >> results1.txt;
	./strassen_basic 1 2048 random.txt >> results1.txt;
	./strassen_basic 1 127 random.txt >> results1.txt;
	./strassen_basic 1 255 random.txt >> results1.txt;
	./strassen_basic 1 511 random.txt >> results1.txt;
	./strassen_basic 1 1023 random.txt >> results1.txt;
	./strassen_basic 1 2047 random.txt >> results1.txt;
	./strassen_basic 1 129 random.txt >> results1.txt;
	./strassen_basic 1 257 random.txt >> results1.txt;
	./strassen_basic 1 513 random.txt >> results1.txt;
	./strassen_basic 1 1025 random.txt >> results1.txt;
	./strassen_basic 1 2049 random.txt >> results1.txt;
	./strassen_basic 1 96 random.txt >> results1.txt;
	./strassen_basic 1 192 random.txt >> results1.txt;
	./strassen_basic 1 384 random.txt >> results1.txt;
	./strassen_basic 1 768 random.txt >> results1.txt;
	./strassen_basic 1 1536 random.txt >> results1.txt

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

strassen_basic.o: strassen_basic.c
	$(CC) $(CFLAGS) -c strassen_basic.c

strassen_basic: $(OBJECTS)
	$(CC) $(OBJECTS) -o strassen_basic -lm

strassen_onepad.o: strassen_onepad.c
	$(CC) $(CFLAGS) -c strassen_onepad.c

strassen_onepad: $(OBJECTO)
	$(CC) $(OBJECTO) -o strassen_onepad -lm

strassen_manypad.o: strassen_manypad.c
	$(CC) $(CFLAGS) -c strassen_manypad.c

strassen_manypad: $(OBJECTM)
	$(CC) $(OBJECTM) -o strassen_manypad -lm

randmat:
	python randmat.py

randmst.o: randmst.c
	$(CC) $(CFLAGS) -c randmst.c

randmst: $(OBJECTR)
	$(CC) $(OBJECTR) -o randmst -lm

clean:
	rm -f *.o strassen_basic randmst strassen_onepad strassen_manypad
