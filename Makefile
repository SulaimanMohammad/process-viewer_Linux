*CC=gcc
CFLAGS= -o3
LDFLAGS= -lprocps       -lpthread

all:myhtop htopexe.o

myhtop: main.o  mycapteur.o     myserver.o myclient.o readproc.o
	$(CC)   $(CFLAGS)       -o      $@      $^      $(LDFLAGS)

mycapteur.o: mycapteur.c        capteur.h
	$(CC)   $(CFLAGS)       -c      -o      $@      $<      $(LDFLAGS)

readproc.o: readprocess.c       readprocess.h
	$(CC)   $(CFLAGS)       -c      -o      $@      $<      $(LDFLAGS)

myserver.o: myserver.c  server.h
	$(CC)   $(CFLAGS)       -c      -o      $@      $<      $(LDFLAGS)

myclient.o: myclient.c  client.h
	 $(CC)   $(CFLAGS)       -c      -o      $@      $<      $(LDFLAGS)

main.o: main.c  capteur.h       readprocess.h   server.h        client.h
	$(CC)   $(CFLAGS) -c    -o $@   $<      $(LDFLAGS)

htopexe.o:htophandy.c     readprocess.o
	$(CC)   $(CFLAGS)       -o      $@      $^


clean:
	rm -rf *.o      myhtop

.PHONY:




