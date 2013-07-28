/*
 * SWIG definition for pyunqliteimp
 */

#ifdef SWIGPYTHON 
%module pyunqliteimp
#endif

%header %{

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

/* Custom routines */
void 
pyunqliteimp_Shutdown()
{
	Py_XDECREF(g_pUnqliteExceptionClass);
}

%} /* %header */


/* Internal mapping generators */
/*%include "stl.i"*/
%include "std_string.i"
%include "exception.i"

/* Create python exception */
%init %{
    g_pUnqliteExceptionClass = PyErr_NewException(const_cast<char*>("_pyunqliteimp.UnqliteException"), NULL, NULL);
    Py_INCREF(g_pUnqliteExceptionClass);
    PyModule_AddObject(m, "UnqliteException", g_pUnqliteExceptionClass);
    
    atexit(pyunqliteimp_Shutdown);
%}

/* General exception handler */
%exception {
  try {
    $action
  } 
  catch (const pyunqlite::UnqliteException& e) {
    PyErr_SetString(g_pUnqliteExceptionClass, const_cast<char*>(e.what()));
    return 0;
  }
}

%pythoncode %{
	UnqliteException = _pyunqliteimp.UnqliteException
%}

/* TODO cleanup cursor */
%feature("ref")   pyunqlite::UnqliteCursor ""
%feature("unref") pyunqlite::UnqliteCursor "delete $this;"

%typemap(newfree) char* "delete($1);";
%newobject pyunqlite::UnqliteDatabaseImp::kv_fetch;


/* Headers to parse to generate wrappers */
%include "UnqliteCursor.h"
%include "UnqliteDatabaseImp.h"
