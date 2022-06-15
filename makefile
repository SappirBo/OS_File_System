all: main

main: main.o myfs.o
	gcc -o main main.o myfs.o

main.o: main.c source/myfs.h
	gcc -c main.c

myfs.o: source/myfs.c source/myfs.h
	gcc -c source/myfs.c

.PHONY: clean #.PHONY means that clean is not a file.

# This clean will clear all the o/a/main files but not the txt files.
clean:
	rm -f *.o *.a  main *.txt
# This Clean is for all the Files.
cleanall:
	rm -f *.o *.a  main *.txt
