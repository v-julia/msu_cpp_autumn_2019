CC=g++

all:
	run

run: sums_by_cols sums_by_rows
	./sums_by_cols
	./sums_by_rows

sums_by_cols: sum_by_cols.o
	$(CC) -o sums_by_cols  sum_by_cols.o -O2
sums_by_rows: sum_by_rows.o
	$(CC) -o sums_by_rows  sum_by_rows.o -O2
sums_by_cols.o: sum_by_cols.cpp sums.h Timer.h
	$(CC) -c sum_by_cols.cpp --std=c++14
sums_by_rows.o: sum_by_rows.cpp sums.h Timer.h
	$(CC) -c sum_by_rows.cpp --std=c++14
 
clean:
	rm -rf *.o
#	rm -rf *.o sums_by_cols
#	rm -rf *.o sums_by_rows
