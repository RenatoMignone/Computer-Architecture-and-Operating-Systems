CC=gcc

main.elf: main.o 
	$(CC) -o main.elf main.o
	
main.o: main.c
	$(CC) -c main.c -o main.o

clean:    
	rm -f *.elf
	rm -f *.o
	rm -f *.i
	rm -f *.s
	rm -f *.out

	
