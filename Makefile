CC=g++
SWIG=swig

THIRDPARTY_DIR=../3rdparty

CFLAGS= -g -fPIC -I/usr/include/python2.7 -I$(THIRDPARTY_DIR)/include
CXXFLAGS=$(CFLAGS)
SWIGFLAGS=-I$(THIRDPARTY_DIR)/include -c++
LDFLAGS= -g -shared -L$(THIRDPARTY_DIR)/lib
LDLIBS= -lunqlite

WRAPPER_LIBRARY=_pyunqliteimp.so

all: wrapper

clean:
	$(RM) *.o *.so *_wrap.cpp pyunqliteimp.py

wrapper: $(WRAPPER_LIBRARY)

$(WRAPPER_LIBRARY): pyunqliteimp.o pyunqliteimp_wrap.o 
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp %.h

%_wrap.cpp: %.i %.cpp %.h 
	$(SWIG) $(SWIGFLAGS) -python -o $@ $<

.PHONY: clean
