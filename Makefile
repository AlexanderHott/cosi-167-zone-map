all: main

main: main.cpp zonemap.cpp 
	g++ -g -std=c++11 -o $@ $^

clear: 
	rm main
