CXX = g++
CXXFLAGS = -std=c++17 -g -O0 -I./include -I"./include/Project 3.0"

SRCDIR = src
BINDIR = bin
TARGET = $(BINDIR)/main
TEST_TARGET = $(BINDIR)/TestDriver
TEST_SOURCE = $(SRCDIR)/Project\ 3.0/TestDriver.cpp
TEST_LINK_SOURCES = "./src/Project 3.0/TestDriver.cpp" "./src/ZipCodeRecord.cpp" "./src/Project 3.0/BlockBuffer.cpp" "./src/Length_Indicated_ZipCodeBuffer.cpp" "./src/Project 3.0/BlockedSequence.cpp" "./src/Project 3.0/Block.cpp" "./src/PrimaryKeyIndex.cpp"

EXCLUDE := $(SRCDIR)/printer.cpp 
EXCLUDE += $(SRCDIR)/main.cpp
EXCLUDE += $(SRCDIR)/HeaderBuffer.cpp
#EXCLUDE += $(SRCDIR)/driver_csv_to_length_indicated.cpp
EXCLUDE += $(SRCDIR)/SearchIndex.cpp

SOURCES := $(filter-out $(EXCLUDE), $(wildcard $(SRCDIR)/*.cpp))

OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

TEST_DEPS = $(SRCDIR)/ZipCodeRecord.cpp
TEST_DEPS += $(SRCDIR)/Project\ 3.0/BlockBuffer.cpp
TEST_DEPS += $(SRCDIR)/Length_Indicated_ZipCodeBuffer.cpp
TEST_DEPS += $(SRCDIR)/Project\ 3.0/BlockedSequence.cpp
TEST_DEPS += $(SRCDIR)/Project\ 3.0/Block.cpp
TEST_DEPS += $(SRCDIR)/PrimaryKeyIndex.cpp

ifeq ($(OS),Windows_NT)
RM_BIN = if exist $(BINDIR) rmdir /s /q $(BINDIR)
else
RM_BIN = rm -rf $(BINDIR)
endif

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

run: $(TARGET)
	./$(TARGET)

$(TEST_TARGET): $(TEST_SOURCE) $(TEST_DEPS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_LINK_SOURCES)

run-test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	$(RM_BIN)

.PHONY: all run run-test clean
