# makefile for whatever.

build:
	g++ -std=c++0x *.cpp *.h -o run_me.out
run: run_me.out
	./run_me.out
