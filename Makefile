SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

TEST_SOURCES=$(wildcard test/*.cpp)
TEST_OBJECTS=$(patsubst %.cpp,%.o,$(TEST_SOURCES))

CFLAGS=-g -O0 -I src
LDFLAGS=-lboost_unit_test_framework

all: test

test: bin/test
	$<

bin/test: $(OBJECTS) $(TEST_OBJECTS) | bin-dir
	g++ -o $@ $^ $(LDFLAGS)
	
%.o: %.cpp
	g++ -c $(CFLAGS) -o $@ $<

bin-dir:
	mkdir -p bin
	
.PHONY: bin-dir 