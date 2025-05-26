CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I.
TARGETS := coder decoder tests

all: $(TARGETS)

coder: coder.cpp shennon_codec.cpp shennon_utils.cpp shennon.h
	$(CXX) $(CXXFLAGS) coder.cpp shennon_codec.cpp shennon_utils.cpp -o $@

decoder: decoder.cpp shennon_codec.cpp shennon_utils.cpp shennon.h
	$(CXX) $(CXXFLAGS) decoder.cpp shennon_codec.cpp shennon_utils.cpp -o $@

tests: tests.cpp shennon_codec.cpp shennon_utils.cpp shennon.h
	$(CXX) $(CXXFLAGS) tests.cpp shennon_codec.cpp shennon_utils.cpp -o $@

test: tests
	./tests

clean:
	rm -f $(TARGETS)

.PHONY: all clean test
