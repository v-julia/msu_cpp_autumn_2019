CC=g++

all: run

test: matrix_test
	./matrix_test

run: test


matrix_test: main.cpp
	$(CC) -o matrix_test  main.cpp  -O2 --std=c++14

clean:
	rm -rf *.o matrix_test

