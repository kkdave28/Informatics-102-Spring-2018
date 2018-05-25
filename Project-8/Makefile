CXXFLAGS = -ggdb -std=c++11 -O3 -pthread

default: twentyeight.cpp twentynine.cpp
	$(CXX) twentyeight.cpp  $(CXXFLAGS) -o twentyeight
	$(CXX) twentynine.cpp $(CXXFLAGS) -o twentynine

28: twentyeight.cpp
	$(CXX) twentyeight.cpp  $(CXXFLAGS) -o twentyeight

29: twentynine.cpp
	$(CXX) twentynine.cpp $(CXXFLAGS) -o twentynine

clean:
	rm -f twentyeight
	rm -f twentynine
	rm -f core.*
	rm -f vgcore.*