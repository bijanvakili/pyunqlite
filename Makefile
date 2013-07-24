CC=g++
SWIG=swig

THIRDPARTY_DIR=../3rdparty
BUILD_DIR=.build
CPPSRC_DIR=src/cpp

CFLAGS= -g -fPIC -I/usr/include/python2.7 -I$(THIRDPARTY_DIR)/include -I$(CPPSRC_DIR)
CXXFLAGS=$(CFLAGS)
SWIGFLAGS=-I$(THIRDPARTY_DIR)/include -c++
LDFLAGS= -g -shared -L$(THIRDPARTY_DIR)/lib
LDLIBS= -lunqlite

WRAPPER_LIBRARY=$(BUILD_DIR)/_pyunqliteimp.so

all: wrapper

dirs:
	mkdir -p $(BUILD_DIR)

clean:
	$(RM) -r $(BUILD_DIR)

wrapper: $(WRAPPER_LIBRARY)

$(WRAPPER_LIBRARY): $(BUILD_DIR)/pyunqliteimp.o $(BUILD_DIR)/pyunqliteimp_wrap.o 
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: $(CPPSRC_DIR)/%.cpp $(CPPSRC_DIR)/%.h dirs
	$(CC) -c $(CXXFLAGS) -o $@ $<

$(BUILD_DIR)/%_wrap.cpp: $(CPPSRC_DIR)/%.i $(CPPSRC_DIR)/%.cpp $(CPPSRC_DIR)/%.h 
	$(SWIG) $(SWIGFLAGS) -python -o $@ $<

.PHONY: clean wrapper dirs
.SECONDARY: $(BUILD_DIR)/pyunqliteimp_wrap.cpp
