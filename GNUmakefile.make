PROGRAMS = randmst

all: $(PROGRAMS)

ALLPROGRAMS = $(PROGRAMS)

include ../common/rules.mk

%.o: %.c $(BUILDSTAMP)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPCFLAGS) $(O) -o $@ -c $<

randmst: randmst.o randmstf.o hexdump.o
	$(CC) $(CFLAGS) $(O) -o $@ $^

clean:
	rm -f $(ALLPROGRAMS) *.o
	rm -rf $(DEPSDIR) *.dSYM

.PHONY: all clean