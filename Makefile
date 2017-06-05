CC = gcc -std=gnu11 -Wall -g -g3 -ggdb -gdwarf-4

.PHONY: all test clean
all: shape.so shape
test: shape
	./shape
clean:
	rm -f shape.so shape

shape.so: shape.c
	$(CC) -fPIC -shared $^ -o $@ -ldl -lm

shape: shape.c
	$(CC) -fPIC $^ -o $@ -ldl -lm
