#OBJS specifies which files to compile as part of the project
OBJS = main.cpp Pieces/Piece.cpp

#CC specifies our compiler
CC = g++

COMPILER_FLAGS = -w -g
# COMPILER_FLAGS = $(shell sdl2-config --cflags)

LINKER_FLAGS = -lSDL2 -I./ -I./Pieces/ -I./Board/
# LINKER_FLAGS = -L/usr/local/lib -l:libSDL2.a -lm -ldl -lpthread -lrt

#OBJ_NAME specifies the name of our executable
OBJ_NAME = tetris

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
