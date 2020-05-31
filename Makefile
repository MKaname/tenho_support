CXX = g++
CXXFLAGS= -std=c++14 `pkg-config opencv --cflags`
LDLIBS = -lpthread `pkg-config opencv --libs`
