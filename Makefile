# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -Wall

all: DFS DFS.exe

DFS.exe : DFS.o
	$(CC) DFS.o -o DFS.exe
	
DFS: DFS.o
	$(CC) DFS.o -o DFS

DFS.o: DFS.cpp
	$(CC) $(CFLAGS) DFS.cpp

clean:
	rm -rf *o DFS.exe
	rm -rf *o DFS
