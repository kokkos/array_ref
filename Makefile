CXX=g++

ifdef DEBUG
	CXXFLAGS+=-O0 -ggdb
else
	CXXFLAGS+=-O3 -fno-omit-frame-pointer 
endif

CXXFLAGS+=-std=c++11 
INCLUDES=
LIBS=
ADDITIONAL_SOURCES=
PROGRAMS=test_build_extents_tuple \
		 test_dims \
		 test_dynamic_extent_tuple_index \
		 test_pack_is_integral
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
	rm $(DIRECTORIES)/*
	rmdir $(DIRECTORIES)
