#ifndef _UNQLITE_BUFFER_H
#define _UNQLITE_BUFFER_H

#include "UnqliteCommon.h"

// forward declarations
#ifndef Py_OBJECT_H
extern "C" {
typedef struct _object PyObject;
}; // extern "C"
#endif // Py_OBJECT_H

namespace pyunqlite
{

// encapsulates a string or byte buffer
class ValueBuffer
{
public:
	ValueBuffer(PyObject* object);
	ValueBuffer(bool is_binary, sxi64 data_len);
	virtual ~ValueBuffer();

	bool is_binary() const;
	char* get_data() const;
	sxi64 get_data_len() const;

	PyObject* get_python_object() const;

protected:
	bool _is_binary;
	PyObject* _object;

	char* _data;
	sxi64 _data_len;

private:
	// disallowed
	ValueBuffer();
	ValueBuffer(const ValueBuffer&);
};

} //namespace pyunqlite

#endif // _UNQLITE_BUFFER_H
