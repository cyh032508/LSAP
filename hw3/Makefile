# 使用 C++17 標準
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17

TARGET = b11705044-compression 
SRCS = main.cpp Huffman.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)