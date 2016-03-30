BASEDIR=$(realpath $(CURDIR)/..)

CXX=g++

ifdef DEBUG
  CXXFLAGS+=-O0 -ggdb
else
  CXXFLAGS+=-O3 -fno-omit-frame-pointer
endif

CXXFLAGS+=-std=c++11
INCLUDES=-I$(BASEDIR)/include
SOURCES=$(shell ls -1 $(CURDIR)/test_*.cpp)
PROGRAMS=$(SOURCES:.cpp=)
DIRECTORIES=$(CURDIR)/build

all: directories $(PROGRAMS)

.PHONY: directories

directories: $(DIRECTORIES)/

$(DIRECTORIES)/:
	@mkdir -p $@ 

% : %.cpp
	@echo "**************************************************"
	@echo "Building $(*F)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $(CURDIR)/build/$(*F)
	@echo "Running $(*F)"
	@build/$(*F)

clean:
	@echo "Cleaning build directory"
	@rm -f $(DIRECTORIES)/*
	@rmdir $(DIRECTORIES)

