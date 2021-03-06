#include <Python.h>
extern "C" {
#include "unqlite.h"
};
#include "UnqliteException.h"
#include "ValueBuffer.h"

namespace pyunqlite {

ValueBuffer::ValueBuffer(PyObject* object)
{
	this->_object = object;

	if (PyString_Check(object)) {
		this->_is_binary = false;
		this->_data_len = PyString_Size(object);
		this->_data = PyString_AsString(object);
	}
	else if (PyByteArray_Check(object)) {
		this->_is_binary = true;
		this->_data_len = PyByteArray_Size(object);
		this->_data = PyByteArray_AsString(object);
	}
	else {
		PyErr_SetString(PyExc_ValueError, "Expecting a string or byte array");
		throw UnqliteException(UNQLITE_INVALID);
	}

	Py_INCREF(this->_object);
}

ValueBuffer::ValueBuffer(bool is_binary, sxi64 data_len)
{
	this->_is_binary = is_binary;

	if (is_binary) {
		this->_object = PyByteArray_FromStringAndSize(0, (Py_ssize_t)data_len);
		if (!this->_object)
			throw UnqliteException(UNQLITE_NOMEM);

		this->_data = PyByteArray_AsString(this->_object);
	}
	else {
		this->_object = PyString_FromStringAndSize(0, (Py_ssize_t)data_len);
		if (!this->_object)
			throw UnqliteException(UNQLITE_NOMEM);

		this->_data = PyString_AsString(this->_object);
	}

	this->_data_len = data_len;
	Py_INCREF(this->_object);
}

ValueBuffer::ValueBuffer(const ValueBuffer& copy)
{
	this->_is_binary = copy._is_binary;
	this->_object = copy._object;
	Py_INCREF(this->_object);
	this->_data = copy._data;
	this->_data_len = copy._data_len;

}

ValueBuffer::~ValueBuffer()
{
	if (this->_object)
	{
		Py_DECREF(this->_object);
		this->_object = 0;
	}
}

bool
ValueBuffer::is_binary() const
{
	return this->_is_binary;
}

char*
ValueBuffer::get_data() const
{
	return this->_data;
}

sxi64
ValueBuffer::get_data_len() const
{
	return this->_data_len;
}

PyObject*
ValueBuffer::get_python_object() const
{
	Py_INCREF(this->_object);
	return this->_object;
}

ValueBuffer::ValueBuffer()
{
}

} // namespace pyunqlite
