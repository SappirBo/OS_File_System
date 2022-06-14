all: main

main: main.o file_system.o
	gcc -o main main.o file_system.o

main.o: main.c file_system.h
	gcc -c main.c

file_system.o: file_system.c file_system.h
	gcc -c file_system.c

.PHONY: clean#.PHONY means that clean is not a file.

# This clean will clear all the o/a/main files but not the txt files.
clean:
	rm -f *.o *.a  main *.txt
# This Clean is for all the Files.
cleanall:
	rm -f *.o *.a  main *.txt
