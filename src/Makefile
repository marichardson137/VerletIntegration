CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lglfw -lGLEW -framework Cocoa -framework OpenGL -framework IOKit

# Path to the glfw and glew libraries
LIB_DIR = dependencies/library

# Path to the include directories of glfw and glew
INCLUDE_DIR = dependencies/include

# Name of the output executable
OUTPUT = app

# Source files and object files
SRC = app.c draw.c
OBJ = $(SRC:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -L $(LIB_DIR) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)

clean:
	rm -f $(OBJ) $(OUTPUT)
