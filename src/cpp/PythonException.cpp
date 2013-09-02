#include <Python.h>
#include "PythonException.h"

namespace pyunqlite {

PythonException::PythonException()
{
	PyErr_Fetch(&_type, &_value, &_traceback);
	Py_XINCREF(this->_type);
	Py_XINCREF(this->_value);
	Py_XINCREF(this->_traceback);
}

PythonException::PythonException(const PythonException& copy)
{
	this->_type = copy._type;
	this->_value = copy._value;
	this->_traceback = copy._traceback;
	Py_XINCREF(this->_type);
	Py_XINCREF(this->_value);
	Py_XINCREF(this->_traceback);
}

PythonException::~PythonException() throw()
{
	Py_XDECREF(this->_type);
	Py_XDECREF(this->_value);
	Py_XDECREF(this->_traceback);
}

// this should never be called
const char*
PythonException::what() const throw()
{
	return "python error";
}

void
PythonException::restore()
{
	if (this->_type)
	{
		PyErr_Restore(this->_type, this->_value, this->_traceback);
		this->_type = 0;
		this->_value = 0;
		this->_traceback = 0;
	}
}

} // namespace pyunqlite
