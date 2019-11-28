CC=g++

all: test

test: vector
	./vector

run: test

vector: main_vector.cpp
	$(CC) -o vector main_vector.cpp -O2 --std=c++14

clean:
	rm -rf *.o vector

