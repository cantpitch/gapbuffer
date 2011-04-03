all: main
	
main: GapBuffer.o main.o

clean:
	- rm *.o main