.PHONY: all clean

all: test

run_tests: bigint.o tests.o
	g++ -std=c++14 bigint.o tests.o -o run_tests

bigint.o: bigint.cpp bigint.hpp
	g++ -std=c++14 -c bigint.cpp -o bigint.o

tests.o: tests.cpp
	g++ -std=c++14 -c tests.cpp -o tests.o

test: run_tests
	./run_tests

clean:
	-(rm run_tests *.o)
