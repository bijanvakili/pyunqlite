#ifndef _UNQLITE_USERCALLBACK_H
#define _UNQLITE_USERCALLBACK_H

#include "UnqliteCommon.h"

namespace pyunqlite
{

class PythonException;

/*
 * User-defined python callback
 *
 * NOTE: Is it expected for the client to call process_exception() to throw any
 *       exceptions caught from the python callback.
 */
class UserCallback
{
public:
	typedef int (*UnqliteDataCallback)(
		const void *pData,
		unsigned int iDataLen,
		void *pUserData
	);

public:
	UserCallback(PyObject* user_callback, bool data_as_binary = false);
	virtual ~UserCallback();

	UnqliteDataCallback get_unqlite_callback_function() const;
	void* get_unqlite_callback_data() const;
	void set_as_binary(bool data_as_binary=true);
	bool get_as_binary() const;

	bool has_exception() const;
	void process_exception();

protected:
	PyObject* _user_callback;
	bool _data_as_binary;
	PythonException* _exception;

	bool route_callback(unsigned int iDataLen,const void *pData);

	// unqlite-defined static callback to route back to the class
	static int OnUnqliteCallback(const void *pData,unsigned int iDataLen,void *pUserData);

private:
	// disallowed
	UserCallback();
};

} // namespace pyunqlite


#endif // _UNQLITE_USERCALLBACK_H
