CXX = g++
CPPFLAGS = -Wall -g

all: rsa

doublet: rsa.cpp
	$(CXX) $(CPPFLAGS) rsa.cpp -o rsa