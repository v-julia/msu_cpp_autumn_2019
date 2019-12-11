CC=g++

all: test

test: sort_thread
	./sort_thread

run: test

sort_thread: sort_thread.cpp
	$(CC) -o sort_thread sort_thread.cpp -lpthread -O2 --std=c++14

clean:
	rm -rf *.o sort_thread

