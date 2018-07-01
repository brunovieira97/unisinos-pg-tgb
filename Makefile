# OBJS specifies which files to compile as part of the project
OBJS = ./Source/*.cpp ./Source/*.c

# CC specifies which compiler we're using
CC = g++ -std=c++11

# COMPILER_FLAGS specifies the additional compilation options we're using
# -Wall will turn on all standard warnings
COMPILER_FLAGS = -Wall

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -I. -I /usr/local/Cellar/ -lglew -lglfw

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)