CXX = g++
CXXFLAGS = -std=c++17 -O2 -g

TARGET = memory

all: $(TARGET)

$(TARGET): memory.o
	$(CXX) $(LDFLAGS) -o $(TARGET) memory.o

memory.o: memory.cpp
	$(CXX) $(CXXFLAGS) -c memory.cpp

clean:
	rm -f *.o $(TARGET)