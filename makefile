# makefile for whatever.

build:
	g++ -std=c++11 *.cpp *.h -o run_me.out
run: a.out
	./run_me.out