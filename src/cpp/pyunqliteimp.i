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
#include "UnqliteException.h"
#include "UnqliteCursor.h"
#include "UnqliteDatabaseImp.h"

/* Custon python exception */
static PyObject* g_pUnqliteExceptionClass;

%}

/* Internal mapping generators */
%include "stl.i"
%include "std_string.i"
%include "exception.i"

/* Create python exception */
%init %{
    g_pUnqliteExceptionClass = PyErr_NewException(const_cast<char*>("_pyunqliteimp.UnqliteException"), NULL, NULL);
    Py_INCREF(g_pUnqliteExceptionClass);
    PyModule_AddObject(m, "UnqliteException", g_pUnqliteExceptionClass);
%}

/* General exception handler */
%exception {
  try {
    $action
  } 
  catch (const pyunqlite::UnqliteException& e) {
    PyErr_SetString(g_pUnqliteExceptionClass, const_cast<char*>(e.what()));
    return NULL;
  }
}

%pythoncode %{
	UnqliteException = _pyunqliteimp.UnqliteException
%}

%feature("ref")   pyunqlite::UnqliteCursor ""
%feature("unref") pyunqlite::UnqliteCursor "delete $this;"

%feature("ref")   pyunqlite::UnqliteDatabaseImp ""
%feature("unref") pyunqlite::UnqliteDatabaseImp "delete $this;"

/* Headers to parse to generate wrappers */
%include "UnqliteCursor.h"
%include "UnqliteDatabaseImp.h"
