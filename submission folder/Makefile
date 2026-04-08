CXX = g++
CXXFLAGS = -std=c++17 -g -O0 -I./include -I"./include/Project 3.0"

SRCDIR = src
BINDIR = bin
TARGET = $(BINDIR)/main
MAIN_SOURCE = $(SRCDIR)/main.cpp
MAIN_LINK_SOURCES = "./src/main.cpp" "./src/ZipCodeRecord.cpp" "./src/Project 3.0/BlockBuffer.cpp" "./src/Length_Indicated_ZipCodeBuffer.cpp" "./src/Project 3.0/BlockedSequence.cpp" "./src/Project 3.0/Block.cpp" "./src/PrimaryKeyIndex.cpp" "./src/HeaderRecord.cpp" "./src/Project 3.0/HeaderBuffer.cpp"
TEST_TARGET = $(BINDIR)/TestDriver
SEARCH_TARGET = $(BINDIR)/SearchIndex
TEST_SOURCE = $(SRCDIR)/Project\ 3.0/TestDriver.cpp
TEST_LINK_SOURCES = "./src/Project 3.0/TestDriver.cpp"
SEARCH_SOURCE = $(SRCDIR)/SearchIndex.cpp
SEARCH_LINK_SOURCES = "./src/SearchIndex.cpp" "./src/PrimaryKeyIndex.cpp" "./src/ZipCodeRecord.cpp" "./src/Project 3.0/BlockBuffer.cpp" "./src/Project 3.0/Block.cpp"

ifeq ($(OS),Windows_NT)
RM_BIN = if exist $(BINDIR) rmdir /s /q $(BINDIR)
MKDIR_BIN = if not exist $(BINDIR) mkdir $(BINDIR)
else
RM_BIN = rm -rf $(BINDIR)
MKDIR_BIN = mkdir -p $(BINDIR)
endif

all: $(TARGET)

$(TARGET): $(MAIN_SOURCE) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_LINK_SOURCES)

$(BINDIR):
	$(MKDIR_BIN)

run: $(TARGET)
	./$(TARGET)

$(TEST_TARGET): $(TEST_SOURCE) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_LINK_SOURCES)

$(SEARCH_TARGET): $(SEARCH_SOURCE) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SEARCH_LINK_SOURCES)

run-test: $(TARGET) $(TEST_TARGET)
	./$(TEST_TARGET)

run-search: $(SEARCH_TARGET)
	./$(SEARCH_TARGET)

clean:
	$(RM_BIN)

.PHONY: all run run-test run-search clean
