CXX = g++
CXXFLAGS = -Wall

all: hamspam clean

hamspam: hamspam.o
	$(CXX) $(CXXFLAGS) hamspam.o -o hamspam

hamspam.o: hamspam.cpp
	$(CXX) -c $(CXXFLAGS) hamspam.cpp

clean:
	rm *.o

cleaner: clean
	rm hamspam

#check bookmarks when editing!
