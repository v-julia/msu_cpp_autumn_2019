CC=g++

all: run test
test: test.py
	python3 test.py

run: calco
	./calco "8*7+743-0+ -2"
	

calco: calco.o
	$(CC) -o calco  calco.cpp -O2 --std=c++14

clean:
	rm -rf *.o calco
