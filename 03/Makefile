.PHONY: all clean

all: test

run_tests: matrix.o tests.o
	g++ -std=c++14 matrix.o tests.o -o run_tests

matrix.o: matrix.cpp matrix.hpp
	g++ -std=c++14 -c matrix.cpp -o matrix.o

tests.o: tests.cpp
	g++ -std=c++14 -c tests.cpp -o tests.o

test: run_tests
	./run_tests

clean:
	-(rm run_tests *.o)
