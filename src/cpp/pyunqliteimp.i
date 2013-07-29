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
  catch (const pyunqlite::UnqliteException& e) {
    PyErr_SetString(g_pUnqliteExceptionClass, const_cast<char*>(e.what()));
    return 0;
  }
}

%pythoncode %{
	UnqliteException = _pyunqliteimp.UnqliteException
%}

/* TODO fix cursor to use ValueBuffer */
%feature("ref")   pyunqlite::UnqliteCursor ""
%feature("unref") pyunqlite::UnqliteCursor "delete $this;"

%typemap(in) sxi64 {
	$1 = PyLong_AsLongLong($input);
}

%typemap(out) sxi64 {
	$result = PyLong_FromLongLong($1);
}

/* Encapsulate 'value' as a string or byte buffer */
%typemap(in) const pyunqlite::ValueBuffer& value {

	if (PyString_Check($input)) {
		$1 = new pyunqlite::ValueBuffer(false, PyString_Size($input), PyString_AsString($input));  
	}
	else if(PyByteArray_Check($input)) {
		$1 = new pyunqlite::ValueBuffer(true, PyByteArray_Size($input), PyByteArray_AsString($input));
	}
	else {
		PyErr_SetString(PyExc_ValueError, "Expecting a string or byte array");
		return 0;
	}
	
	if (!$1) {
		PyErr_SetString(PyExc_MemoryError, "Unable to allocate metadata for buffer");
		return 0;
	}
}

%typemap(freearg) const pyunqlite::ValueBuffer& value {
	delete $1;
}

%typemap(out) pyunqlite::ValueBuffer* {
	if ($1->is_binary()) {
		$result = PyByteArray_FromStringAndSize(
			static_cast< const char* >($1->get_data()),
			$1->get_data_len()
		);
	}
	else {
		$result = PyString_FromStringAndSize(
			static_cast< const char* >($1->get_data()),
			$1->get_data_len()
		);
	}
	
	delete $1;
}

/* Headers to parse to generate wrappers */
%include "UnqliteCursor.h"
%include "UnqliteDatabaseImp.h"
