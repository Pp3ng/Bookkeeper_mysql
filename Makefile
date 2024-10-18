# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -I/usr/include/mysql -I/usr/include/openssl

# Linker flags
LDFLAGS = -L/usr/lib/mysql -lmysqlclient -lcrypto -lssl

# Source files (automatically detect all .cpp files)
SRCS = $(wildcard *.cpp)

# Object files directory
OBJDIR = build

# Object files
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

# Executable name
TARGET = bookkeeper

# Default target
all: $(TARGET)

# Create object files directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Rule to link object files and create the executable
$(TARGET): $(OBJDIR) $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile source files into object files
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "Usage:"
	@echo "  make         - Build the project"
	@echo "  make clean   - Clean the build files"
	@echo "  make run     - Run the program"
	@echo "  make help    - Show this help message"

# Phony targets
.PHONY: all clean run help
