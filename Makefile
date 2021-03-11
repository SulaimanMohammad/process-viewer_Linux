CC=gcc
CFLAGS= -o3
LDFLAGS= -lprocps

all:	myhtop

myhtop:	main.o	mycapteur.o
	$(CC)	$(CFLAGS)	-o	$@	$^	$(LDFLAGS)

mycapteur.o: mycapteur.c	capteur.h
	$(CC)	$(CFLAGS)	-c	-o	$@	$<	$(LDFLAGS)

main.o: main.c  capteur.h
	$(CC)   $(CFLAGS) -c	-o $@	$<	$(LDFLAGS)

clean:
	rm -rf *.o	myhtop

.PHONY:
