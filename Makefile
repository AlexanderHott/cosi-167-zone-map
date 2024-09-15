CXX = g++
CFLAGS = -Wall -g -std=c++20


all: main
.PHONY: all

main: main.cpp
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -rf main
.PHONY: clean
