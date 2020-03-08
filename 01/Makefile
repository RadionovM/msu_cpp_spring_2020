.PHONY: all clean

all: test

test_bin: allocator.cpp
	g++ -std=c++14 allocator.cpp -o test_bin

test: test_bin
	./test_bin

clean:
	-(rm test_bin)
