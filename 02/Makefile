.PHONY: all clean

all: test

run_tests: parser.o tests.o
	g++ -std=c++14 parser.o tests.o -o run_tests

parser.o: parser.cpp parser.hpp
	g++ -std=c++14 -c parser.cpp -o parser.o

tests.o: tests.cpp
	g++ -std=c++14 -c tests.cpp -o tests.o

test: run_tests
	./run_tests

clean:
	-(rm run_tests *.o)
