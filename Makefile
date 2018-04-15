all:ReadWrite

ReadWrite:ReadWrite.c
	gcc -pthread ReadWrite.c -o ReadWrite

clean:
	rm -f ReadWrite
