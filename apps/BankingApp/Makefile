CXX = g++

CXXFLAGS = -std=c++20 -Wall -Wextra

INCLUDES = -I../../common/include

LIBS = -L../../lib -lCommon

TARGET = ../../bin/BankingSystem

SRC = main.cpp

all:
	mkdir -p ../../bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
