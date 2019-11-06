CC=g++

all: test

test: big_int_test
	./big_int_test

run: test

big_int_test: main.cpp bigint.cpp
	$(CC) -o big_int_test main.cpp bigint.cpp -O2 --std=c++14

clean:
	rm -rf *.o big_int big_int_test

