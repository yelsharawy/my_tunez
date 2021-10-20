all: main.o songs.o
	gcc -o my_tunez main.o songs.o

main.o: main.c songs.h
	gcc -c main.c

songs.o: songs.c songs.h
	gcc -c songs.c

clean:
	-rm ./*.o
	-rm ./my_tunez*

run:
	./my_tunez*
