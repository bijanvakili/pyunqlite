/*
 * SWIG definition for pyunqlite
 */

#ifdef SWIGPYTHON 
%module pyunqlite
#endif


/*
typedef unsigned long long uint64_t;
%feature("ref")   UnqliteDatabase "$this->ref();"
%feature("unref") UnqliteDatabase "$this->unref();"
*/

%{
#define SWIG
#define SWIG_FILE_WITH_INIT

#include "unqlite.h"

%}

%include "unqlite.h"
