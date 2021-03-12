	
all:
	gcc -c readprocess.c readprocess.h
	gcc -o3 htophandy.c readprocess.c -o htopexe.o
clean:
	rm -f *.o
