CC = gcc
CFLAGS = -Wall -std=c99 -O2
LDFLAGS = -lpthread -lglfw -lGLEW -framework Cocoa -framework OpenGL -framework IOKit

# Path to the glfw and glew libraries
LIB_DIR = src/dependencies/library

# Path to the include directories of glfw and glew
INCLUDE_DIR = src/dependencies/include

# Name of the output executable
OUTPUT = app

# Source files and object files
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -L $(LIB_DIR) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)

debug: CFLAGS += -DDEBUG -O0 -g
debug: clean $(OUTPUT)

clean:
	rm -f $(OBJ) $(OUTPUT)
