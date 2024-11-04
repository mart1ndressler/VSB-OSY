CXX = g++
CXXFLAGS = -Wall -std=c++11 -fPIC

all: program program1 program1_hex

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

# Druhý úkol - Sestavení pro hex čísla
program1_hex: main1.o libinput_hex.so
	$(CXX) $(CXXFLAGS) -o program1_hex main1.o -L. -linput_hex

libinput_hex.so: function1_hex.o
	$(CXX) -shared -o libinput_hex.so function1_hex.o

function1_hex.o: function1_hex.cpp header1.h
	$(CXX) $(CXXFLAGS) -c function1_hex.cpp

# Úklid
clean:
	rm -f *.o libfunctions.a libinput.so libinput_hex.so program program1 program1_hex