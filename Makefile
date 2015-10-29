SOURCE_DIR=src
BUILD_DIR=build
BIN_DIR=bin

SOURCES=$(filter-out $(SOURCE_DIR)/main.cpp $(SOURCE_DIR)/%Test.cpp,$(wildcard $(SOURCE_DIR)/*.cpp))
OBJECTS=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
OBJECT_DEPS=$(patsubst %.o,%.d,$(OBJECTS)) $(BUILD_DIR)/main.d $(BUILD_DIR)/BoardTest.d $(BUILD_DIR)/LiterakiBoardTest.d

CFLAGS=-g -O0 -I src -std=c++0x
LDFLAGS=-lboost_unit_test_framework -std=c++0x

all: bin/board-printer-test bin/scribble bin/unit-test

test: bin/unit-test
	$<

Makefile: $(OBJECT_DEPS)

$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.cpp | dir.$(BUILD_DIR)
	g++ -MM -MT $*.deps -o $@ $(CFLAGS) $^

-include $(OBJECT_DEPS)

$(BIN_DIR)/unit-test: $(OBJECTS) $(BUILD_DIR)/BoardTest.o | dir.$(BIN_DIR)
	g++ -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/board-printer-test: $(OBJECTS) $(BUILD_DIR)/LiterakiBoardTest.o | dir.$(BIN_DIR)
	g++ -o $@ $^ $(LDFLAGS) 

$(BIN_DIR)/scribble: $(SOURCE_DIR)/main.cpp $(OBJECTS) | dir.$(BIN_DIR)
	g++ -o $@ $^ $(LDFLAGS) 

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp | %.deps dir.$(BUILD_DIR)
	g++ -o $@ $(CFLAGS) -c $<

dir.%:
	@mkdir -p $*
