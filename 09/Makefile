.PHONY: all clean

all: test

run_tests: tests.o
	g++ -g -std=c++17 -pthread tests.o -o run_tests

tests.o: tests.cpp 
	g++ -g -std=c++17 -c tests.cpp -o tests.o

test: run_tests
	-(rm .*.dat)
	-(rm *.dat)
	./run_tests

clean:
	-(rm .*.dat)
	-(rm *.dat)
	-(rm run_tests *.o)
