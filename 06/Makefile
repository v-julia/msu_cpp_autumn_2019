CC=g++

all: test

test: format
	./format

run: test

format: main_format.cpp
	$(CC) -o format main_format.cpp -O2 --std=c++14

clean:
	rm -rf *.o format

