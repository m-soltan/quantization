CC = gcc
FLAGS = -Wall -Wextra -std=c11

# all: main

main: main.o parser.o trie.o energy.o
	$(CC) $(FLAGS) -o $@ $^

# quantization: main.c parser.o
# 	$(CC) -Wall -Wextra -std=c11 -O2 main.c parser.o trie.o energy.o 

parser.o: parser.c parser.h
	$(CC) $(FLAGS) -c $^

trie.o: trie.c trie.h
	$(CC) $(FLAGS) -c $^

energy.o: energy.c energy.h
	$(CC) $(FLAGS) -c $^

main.o: main.c parser.h trie.h energy.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.gch *.o main