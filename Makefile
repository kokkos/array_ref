CXX=g++

ifdef DEBUG
	CXXFLAGS+=-O0 -ggdb
else
	CXXFLAGS+=-O3 -fno-omit-frame-pointer 
endif

CXXFLAGS+=-std=c++11 
INCLUDES=
LIBS=
SOURCES=$(shell ls -1 test_*.cpp)
ADDITIONAL_SOURCES=
PROGRAMS=$(SOURCES:.cpp=)
DIRECTORIES=build

all: directories $(PROGRAMS)

.PHONY: directories
directories: $(DIRECTORIES)/  

$(DIRECTORIES)/:
	mkdir -p $@ 

% : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) $(ADDITIONAL_SOURCES) $< -o build/$@
	build/$@

clean:
	rm -f $(DIRECTORIES)/*
	rmdir $(DIRECTORIES)
