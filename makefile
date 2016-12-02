all: main.o executor.o stringext.o
	gcc main.o executor.o stringext.o -o shell

main: main.c
	gcc -c main.c 

executor: executor.c executor.h
	gcc -c executor.c

stringext: stringext.c stringext.h
	gcc -c stringext.c

run: shell
	./shell

clean:
	rm *~
