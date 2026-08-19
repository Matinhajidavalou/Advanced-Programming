CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

SRCS = main.cpp Utils.cpp System.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = UTTrade

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)