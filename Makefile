SOURCES=$(filter-out src/main.cpp,$(wildcard src/*.cpp))
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

TEST_SOURCES=$(wildcard test/*.cpp)
TEST_OBJECTS=$(patsubst %.cpp,%.o,$(TEST_SOURCES))

CFLAGS=-g -O0 -I src -std=c++0x
LDFLAGS=-lboost_unit_test_framework -std=c++0x

all: test bin/board-printer-test bin/scribble

test: bin/unit-test
	$<

bin/unit-test: $(OBJECTS) test/BoardTest.o | bin-dir
	g++ -o $@ $^ $(LDFLAGS)

bin/board-printer-test: $(OBJECTS) test/LiterakiBoardTest.o
	g++ -o $@ $^ $(LDFLAGS) 

bin/scribble: src/main.cpp $(OBJECTS) | bin-dir
	g++ -o $@ $^ $(LDFLAGS) 

%.o: %.cpp
	g++ -c $(CFLAGS) -o $@ $<

bin-dir:
	mkdir -p bin

.PHONY: bin-dir 