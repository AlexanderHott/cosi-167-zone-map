CXX = g++
CFLAGS = -Wall -g -std=c++17


all: main
.PHONY: all

main: main.cpp zonemap.h
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -rf main
.PHONY: clean
