CC=gcc
CFLAGS= -o3

all:	myhtop

myhtop:	main.o	mycapteur.o
	$(CC)	$(CFLAGS)	-o	$@	$^

mycapteur.o: mycapteur.c	capteur.h
	$(CC)	$(CFLAGS)	-c	-o	$@	$<

main.o: main.c  capteur.h
	$(CC)   $(CFLAGS) -c	-o $@	$<

clean:
	rm -rf *.o	myhtop

.PHONY:
