.PHONY: all clean

all: test

run_tests: allocator.o tests.o
	g++ -std=c++14 allocator.o tests.o -o run_tests

allocator.o: allocator.cpp allocator.hpp
	g++ -std=c++14 -c allocator.cpp -o allocator.o

tests.o: tests.cpp
	g++ -std=c++14 -c tests.cpp -o tests.o

test: run_tests
	./run_tests

clean:
	-(rm run_tests *.o)
