/*
 * SWIG definition for pyunqliteimp
 */

#ifdef SWIGPYTHON 
%module (docstring="Client library for the UnQLite") pyunqliteimp
#endif

%header %{

#define SWIG_FILE_WITH_INIT

/* Internal headers for wrapper */
#include <exception>
#include <string>
extern "C" {
	#include "unqlite.h"
};
#include "UnqliteException.h"
#include "PythonException.h"
#include "UnqliteCursor.h"
#include "UnqliteVirtualMachine.h"
#include "UnqliteDatabaseImp.h"
#include "ValueBuffer.h"

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
%include "std_string.i"
%include "exception.i"

/* Create a custom python exception (UnqliteException) */
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
  catch (pyunqlite::PythonException& e) {
  	e.restore();
  	return 0;
  }
  catch (const pyunqlite::UnqliteException& e) {
    PyErr_SetString(g_pUnqliteExceptionClass, const_cast<char*>(e.what()));
    return 0;
  }
}

/* python docstr generators */
%feature("autodoc", "2");
%define DOCSTRING(node, description)
	%feature("docstring", description) node;
%enddef

%define DOCSTRING_ARG(type, description)
	%typemap("doc") type "$1_name: "description
%enddef


%newobject pyunqlite::UnqliteDatabaseImp::kv_cursor;
%feature("ref")   pyunqlite::UnqliteCursor ""
%feature("unref") pyunqlite::UnqliteCursor "delete $this;"

%newobject pyunqlite::UnqliteDatabaseImp::vm_compile;
%feature("ref")   pyunqlite::UnqliteVirtualMachine ""
%feature("unref") pyunqlite::UnqliteVirtualMachine "delete $this;"

%typemap(in) sxi64 {
	$1 = PyLong_AsLongLong($input);
}

%typemap(out) sxi64 {
	$result = PyLong_FromLongLong($1);
}

%include "valuebuffer.i"
%include "usercallback.i"

/* Headers to parse to generate wrappers */
%include "UnqliteCursor.h"
%include "UnqliteVirtualMachine.h"
%include "UnqliteDatabaseImp.h"
