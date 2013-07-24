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
%include "exception.i"

/* General exception handler */
/* TODO Create a python exception 'UnqliteException' */
%exception {
  try {
    $action
  } 
  catch (const pyunqlite::UnqliteException& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

%feature("ref")   pyunqlite::UnqliteCursor ""
%feature("unref") pyunqlite::UnqliteCursor "delete $this;"

%feature("ref")   pyunqlite::UnqliteDatabaseImp ""
%feature("unref") pyunqlite::UnqliteDatabaseImp "delete $this;"

/* Headers to parse to generate wrappers */
%include "pyunqliteimp.h"

