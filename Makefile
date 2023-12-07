CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
LDFLAGS = -lboost_system -pthread

# Target executables
TARGETS = server client

all: $(TARGETS)

server: server.cpp
	$(CXX) $(CXXFLAGS) -o server server.cpp $(LDFLAGS)

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp $(LDFLAGS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean