CC = gcc
CFLAGS = -g -pthread

TESTE = sudoku

all: $(TESTE)

sudoku: sudoku_dicas.c sudoku_teste.c
	$(CC) $(CFLAGS) sudoku_teste.c -o sudoku_teste
	$(CC) $(CFLAGS) sudoku_dicas.c -o sudoku_dicas

clean:
	rm -f *~ *.o $(TESTE)

