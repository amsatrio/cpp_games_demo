CC = g++
# Added -I. to ensure headers in the current directory are found easily
CFLAGS = -Wall -std=c++23 -I. `pkg-config --cflags sdl3 sdl3-image`
LIBS = `pkg-config --libs sdl3 sdl3-image`

# List your source files here
SRCS = main.cpp game.cpp entity.cpp
# This automatically generates a list of .o files from your .cpp files
OBJS = $(SRCS:.cpp=.o)
TARGET = main.bin

all: $(TARGET)

# Link the object files into the final binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

# Compile each source file into an object file
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean start

start:
	rm -rf build
	mkdir -p build
	cd build && cmake ..
	cd build && $(MAKE)
	cd build && ./main.bin