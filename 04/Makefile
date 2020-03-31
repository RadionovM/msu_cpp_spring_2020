.PHONY: all clean

all: test

run_tests: tests.cpp serializer.hpp
	g++ -std=c++14 tests.cpp -o run_tests

test: run_tests
	./run_tests

clean:
	-(rm run_tests)
