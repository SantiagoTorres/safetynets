CXX = g++
CXXFLAGS = -O3

all: test

test: safetynets.cc
	$(CXX) $(CXXFLAGS) -o safetynets.o safetynets.cc math.cc util.cc

clean:
	rm *.o
