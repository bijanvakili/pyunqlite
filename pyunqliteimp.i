/*
 * SWIG definition for pyunqliteimp
 */

#ifdef SWIGPYTHON 
%module pyunqliteimp
#endif

%{
#define SWIG
#define SWIG_FILE_WITH_INIT

/* Internal headers for wrapper */
#include <exception>
#include <string>
extern "C" {
	#include "unqlite.h"
};
#include "pyunqliteimp.h"

%}

/* Internal mapping generators */
%include "stl.i"
%include "std_string.i"


/* Headers to parse to generate wrappers */
%include "pyunqliteimp.h"
