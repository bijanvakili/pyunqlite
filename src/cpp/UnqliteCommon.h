#ifndef _UNQLITE_COMMON_H
#define _UNQLITE_COMMON_H

// forward declarations
extern "C" {
typedef struct unqlite unqlite;
}; // extern "C"

#ifndef _UNQLITE_H_

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed __int64     sxi64; /* 64 bits(8 bytes) signed int64 */
typedef unsigned __int64   sxu64; /* 64 bits(8 bytes) unsigned int64 */
#else
typedef signed long long int   sxi64; /* 64 bits(8 bytes) signed int64 */
typedef unsigned long long int sxu64; /* 64 bits(8 bytes) unsigned int64 */

#endif /* _MSC_VER */

#endif // _UNQLITE_H_

// forward declarations
#ifndef Py_OBJECT_H
extern "C" {
typedef struct _object PyObject;
}; // extern "C"
#endif // Py_OBJECT_H


#endif // _UNQLITE_COMMON_H
