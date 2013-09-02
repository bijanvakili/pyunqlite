
SWIG=@SWIGBIN@
SWIGFLAGS= @CPPFLAGS@ -c++ -builtin -keyword

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = _pyunqliteimp.la
_pyunqliteimp_la_LDFLAGS = -module

_pyunqliteimp_la_CXXFLAGS= -I. @CXXFLAGS@ @CPPFLAGS@
_pyunqliteimp_la_LDLIBS= -lunqlite

_pyunqliteimp_la_headers=UnqliteCommon.h \
	UnqliteDatabaseImp.h \
	UnqliteCursor.h \
	UnqliteException.h \
	ValueBuffer.h

_pyunqliteimp_la_SOURCES = \
	pyunqliteimp_wrap.cpp \
	UnqliteDatabaseImp.cpp \
	UnqliteCursor.cpp \
	UnqliteException.cpp \
	ValueBuffer.cpp \
	$(_pyunqliteimp_la_headers)
BUILT_SOURCES=pyunqliteimp_wrap.cpp

pyunqliteimp.i: valuebuffer.i 

pyunqliteimp_wrap.cpp: pyunqliteimp.i $(_pyunqlite_la_headers)
	$(SWIG) $(SWIGFLAGS) -python -o $@ $<

.SECONDARY: pyunqliteimp_wrap.cpp
