.PHONY: all clean

all: test

run_tests:  tests.o
	g++ -std=c++14  tests.o -o run_tests


tests.o: tests.cpp vector.hpp
	g++ -std=c++14 -c tests.cpp -o tests.o

test: run_tests
	./run_tests

clean:
	-(rm run_tests *.o)
