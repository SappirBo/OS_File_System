all: main

main: main.o file_system.o
	gcc -o main main.o file_system.o

main.o: main.c file_system.h
	gcc -c main.c

file_system.o: file_system.c file_system.h
	gcc -c file_system.c

.PHONY: clean#.PHONY means that clean is not a file.
clean:
	rm -f *.o *.a  main fd_data.txt
