CXX = g++
CXXFLAGS = -std=c++17 -I./include

SRCDIR = src
BINDIR = bin
TARGET = $(BINDIR)/main

EXCLUDE := $(SRCDIR)/printer.cpp 
EXCLUDE += $(SRCDIR)/main.cpp
EXCLUDE += $(SRCDIR)/HeaderBuffer.cpp
#EXCLUDE += $(SRCDIR)/driver_csv_to_length_indicated.cpp
EXCLUDE += $(SRCDIR)/SearchIndex.cpp

SOURCES := $(filter-out $(EXCLUDE), $(wildcard $(SRCDIR)/*.cpp))

OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BINDIR)

.PHONY: all run clean
