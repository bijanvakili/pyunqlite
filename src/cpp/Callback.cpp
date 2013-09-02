#include <Python.h>
extern "C" {
#include "unqlite.h"
};
#include "PythonException.h"
#include "UnqliteException.h"
#include "Callback.h"

namespace pyunqlite {


UserCallback::UserCallback()
{
	this->_user_callback = 0;
	this->_data_as_binary = false;
	this->_exception = 0;
}


UserCallback::UserCallback(PyObject* user_callback,	bool data_as_binary)
{
	// check preconditions
	if (!user_callback)
	{
		PyErr_SetString(PyExc_TypeError, "callback parameter not specified");
		throw UnqliteException(UNQLITE_INVALID);
	}
    if (!PyCallable_Check(user_callback) || !PyFunction_Check(user_callback))
    {
		PyErr_SetString(PyExc_TypeError, "callback parameter is not callable");
		throw UnqliteException(UNQLITE_INVALID);
	}

    // store a reference to the callback
	this->_user_callback = user_callback;
	Py_INCREF(this->_user_callback);
	set_as_binary(data_as_binary);

	this->_exception = 0;
}


UserCallback::~UserCallback()
{
	delete this->_exception;
	Py_XDECREF(this->_user_callback);
}


void*
UserCallback::get_unqlite_callback_data() const
{
	return (void*)this;
}


UserCallback::UnqliteDataCallback
UserCallback::get_unqlite_callback_function() const
{
	return OnUnqliteCallback;
}


void
UserCallback::set_as_binary(bool data_as_binary)
{
	this->_data_as_binary = data_as_binary;
}


bool
UserCallback::get_as_binary() const
{
	return this->_data_as_binary;
}

bool
UserCallback::has_exception() const
{
	return (this->_exception != 0);
}

void
UserCallback::process_exception()
{
	if (has_exception())
	{
		throw *(this->_exception);
	}
}


bool
UserCallback::route_callback(unsigned int iDataLen,const void *pData)
{
	PyObject* arglist = 0;
	PyObject* result = 0;
#if(PY_MAJOR_VERSION<3)
	PyObject* bytearray = 0;
#endif // PY_MAJOR_VERSION
	bool bContinue = false;

	// setup the parameters to pass to the user's python function
	if (this->_data_as_binary)
	{
		/*
		 * Python 2.x has no 'bytes' type, so use mutable 'bytearray' which copies data
		 * Python 3.x has 'bytes' type, which is immutable and requires no copy
		 */

#if(PY_MAJOR_VERSION>=3)
		arglist = Py_BuildValue("(y#i)", pData, iDataLen, iDataLen);
#else
		bytearray = PyByteArray_FromStringAndSize(reinterpret_cast<const char*>(pData), iDataLen);
		arglist = Py_BuildValue("(Oi)", bytearray, iDataLen);
#endif // PY_MAJOR_VERSION
	}
	else
	{
		arglist = Py_BuildValue("(s#i)", pData, iDataLen, iDataLen);
	}

	if (!arglist)
	{
		this->_exception = new PythonException();
	}
	else {
		// route to the python callback
		result = PyObject_CallObject(this->_user_callback, arglist);

		// determine if we should continue processing
		if (!result)
			this->_exception = new PythonException();
		else if (PyBool_Check(result))
			bContinue = (result == Py_True);
	}

	// cleanup
#if(PY_MAJOR_VERSION<3)
	Py_XDECREF(bytearray);
#endif // PY_MAJOR_VERSION
	Py_XDECREF(result);
	Py_XDECREF(arglist);

	return bContinue;
}


// unqlite-defined static callback to route back to the class
int
UserCallback::OnUnqliteCallback(const void *pData,unsigned int iDataLen,void *pUserData)
{
	// route to internal method
	bool bContinue = reinterpret_cast<UserCallback*>(pUserData)->route_callback(
		iDataLen,
		pData
	);
	return bContinue ? UNQLITE_OK : UNQLITE_ABORT;
}

} // namespace pyunqlite
