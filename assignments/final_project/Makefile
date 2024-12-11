# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Output executable
TARGET = raid

# Files to compile
SRC = raid.cpp

# Default target
all: $(TARGET)

# Link the object files into the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Perform RAID 10 on the mock data
raid10:
	./$(TARGET) raid10
	@echo "RAID 10 operation completed."

# Reconstruct the RAID 10 files
reconstruct:
	./$(TARGET) reconstruct
	@echo "RAID 10 files reconstruction completed."

# Clean the generated files (delete .txt)
clean:
	./$(TARGET) clean
	@echo "Clean operation completed. All files removed."
