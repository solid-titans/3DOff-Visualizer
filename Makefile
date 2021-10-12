OBJ=3dViewer.out
MAIN=src/main.cpp
SOURCES=$(wildcard *.c)
CFLAGS=-g -Wall -Wextra `pkg-config --cflags --libs Qt5Widgets` -fPIC
EXECUTABLES=$(patsubst %.c,%,$(SOURCES))

all:
	g++ $(CFLAGS) -o $(OBJ) $(MAIN)

run: all
	clear
	./$(OBJ)
