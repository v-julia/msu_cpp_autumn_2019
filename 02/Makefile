CC=g++

all: run

test: alloc_test
	./alloc_test

run: test


alloc_test: aloc_main.cpp LinearAllocator.cpp
	$(CC) -o alloc_test  LinearAllocator.cpp aloc_main.cpp -O2 --std=c++14

clean:
	rm -rf *.o alloc_test

