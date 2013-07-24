#ifndef _UNQLITE_EXCEPTION_H
#define _UNQLITE_EXCEPTION_H

#include <string>
#include <exception>
#include "UnqliteCommon.h"

namespace pyunqlite {

// unqlite exception class
class UnqliteException: public std::exception
{
public:
	UnqliteException(int error_code, unqlite* pDB = 0);
	UnqliteException(const std::string& error_text, unqlite* pDB = 0);
	virtual ~UnqliteException() throw();

	virtual const char* what() const throw();

protected:
	std::string _error_text;

	virtual void AppendLogError(unqlite* pDB = 0);

private:
	UnqliteException();
};


} // namespace pyunqlite

#endif // _UNQLITE_EXCEPTION_H
