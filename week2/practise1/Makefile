CXX = g++
CXXFLAGS = -Wall -std=c++11 -fPIC

all: program program1 program1_float

# První úkol - Statická knihovna
program: main.o libfunctions.a
	$(CXX) $(CXXFLAGS) -o program main.o -L. -lfunctions

main.o: main.cpp header.h
	$(CXX) $(CXXFLAGS) -c main.cpp

libfunctions.a: function.o
	ar rcs libfunctions.a function.o

function.o: function.cpp header.h
	$(CXX) $(CXXFLAGS) -c function.cpp

# Druhý úkol - Sestavení pro celá čísla
program1: main1.o libinput.so
	$(CXX) $(CXXFLAGS) -o program1 main1.o -L. -linput

main1.o: main1.cpp header1.h
	$(CXX) $(CXXFLAGS) -c main1.cpp

libinput.so: function1.o
	$(CXX) -shared -o libinput.so function1.o

function1.o: function1.cpp header1.h
	$(CXX) $(CXXFLAGS) -c function1.cpp

# Druhý úkol - Sestavení pro float čísla
program1_float: main1.o libinput_float.so
	$(CXX) $(CXXFLAGS) -o program1_float main1.o -L. -linput_float

libinput_float.so: function1_float.o
	$(CXX) -shared -o libinput_float.so function1_float.o

function1_float.o: function1_float.cpp header1.h
	$(CXX) $(CXXFLAGS) -c function1_float.cpp

# Úklid
clean:
	rm -f *.o libfunctions.a libinput.so libinput_float.so program program1 program1_float
