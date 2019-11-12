CC=g++

all: test

test: ser_test
	./ser_test

run: test

ser_test: test.cpp
	$(CC) -o ser_test test.cpp -O2 --std=c++14

clean:
	rm -rf *.o ser_test

