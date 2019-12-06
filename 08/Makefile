CC=g++

all: test

test: main_treads
	./main_treads

run: test

main_treads: main_treads.cpp
	$(CC) -o main_treads main_treads.cpp -O2 --std=c++14

clean:
	rm -rf *.o main_treads

