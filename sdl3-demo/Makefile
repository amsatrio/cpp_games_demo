BUILD_DIR = build
TARGET = $(BUILD_DIR)/src/sdl3-demo
SRC = src/main.cpp
CMAKE_FILES = CMakeLists.txt src/CMakeLists.txt

.PHONY: all clean start

all: $(TARGET)

$(TARGET): $(SRC) $(CMAKE_FILES)
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..
	@$(MAKE) -C $(BUILD_DIR)

start: $(TARGET)
	@echo "Running $(TARGET) — close the window to exit."
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
