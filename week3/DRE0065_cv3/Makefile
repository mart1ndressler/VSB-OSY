CXX = g++
CXXFLAGS = -Wall

all: myls mygen monitor

myls: ls.cpp function.cpp header.h
	$(CXX) $(CXXFLAGS) -o myls ls.cpp function.cpp

mygen: gen.cpp function.cpp header.h
	$(CXX) $(CXXFLAGS) -o mygen gen.cpp function.cpp

monitor: monitor.cpp function.cpp header.h
	$(CXX) $(CXXFLAGS) -o monitor monitor.cpp function.cpp

clean:
	rm -f myls mygen monitor