CXX = g++
PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

MKL_ROOT = /opt/intel/compilers_and_libraries_2017.5.239/linux/mkl/
MKL_PATH = $(MKL_ROOT)lib/intel64/
INCLUDES = -I$(MKL_ROOT)include

LIBS = -lmkl_intel_ilp64 \
	-lmkl_sequential \
	-lmkl_core \
	-lm

OBJS = mkl_test.o

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g -DMKL_ILP64
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O3 -DMKL_ILP64
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all:	mkl_test

mkl_test:	$(OBJS)
	$(CXX) $(INCLUDES) -L$(MKL_PATH) -Wl,--no-as-needed $(LIBS)  -o $@ $^

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $< $(INCLUDES)

clean:
	rm -fr mkl_test $(OBJS)
