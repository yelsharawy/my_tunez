all: main.o songs.o songlib.o
	gcc -o my_tunez main.o songs.o songlib.o

main.o: main.c songs.h songlib.h
	gcc -c main.c

songs.o: songs.c songs.h
	gcc -c songs.c

songlib.o: songlib.c songlib.h
	gcc -c songlib.c

clean:
	-rm ./*.o
	-rm ./my_tunez*

run:
	./my_tunez*
