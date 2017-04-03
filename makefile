all: run

run: compile
	./keks
compile:
	gcc keks.c -o keks
