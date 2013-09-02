#ifndef _PYTHON_EXCEPTION_H
#define _PYTHON_EXCEPTION_H

#include <string>
#include <exception>
#include "UnqliteCommon.h"

namespace pyunqlite {

/*
 * Exception class for propagating python errors (e.g. from callbacks)
 */
class PythonException: public std::exception
{
public:
	PythonException();
	PythonException(const PythonException& copy);
	virtual ~PythonException() throw();

	const char*	what() const throw();
	void restore();

protected:
	PyObject* _type;
	PyObject* _value;
	PyObject* _traceback;
};

} // namespace pyunqlite

#endif // _PYTHON_EXCEPTION_H
