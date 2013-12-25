#ifndef _UNQLITE_VIRTUALMACHINE_H
#define _UNQLITE_VIRTUALMACHINE_H

#include <string>
#include <map>
#include "UnqliteCommon.h"
#include "UnqliteException.h"
#include "Callback.h"

namespace pyunqlite
{

// virtual machine implementation
class UnqliteVirtualMachine
{
public:
	UnqliteVirtualMachine(unqlite* db, unqlite_vm* vm);
	virtual ~UnqliteVirtualMachine();

	DOCSTRING(close, "Explicitly releases the virtual machine.")
	virtual void close();

	DOCSTRING(execute, "Executes the virtual machine.")
	DOCSTRING_ARG(pyunqlite::UserCallback* callback, "Optional data consumer callback")
	DOCSTRING_ARG(bool as_binary, "If set to 'true', data will be retrieved as a 'bytearray'")
	virtual void execute(
		pyunqlite::UserCallback* callback = 0,
		bool callback_as_binary = false
	);

	DOCSTRING(reset, "Resets the virtual machine to its initial state.")
	virtual void reset();

	DOCSTRING(create_constant, "Adds a constant to the virtual machine.")
	DOCSTRING_ARG(const char* name, "Name of the constant as accessed by the virtual machine")
	DOCSTRING_ARG(Py::Callable& callback, "User-defined callback function")
	DOCSTRING_ARG(Py::Callable& callback, "Optional user data for the callback")
	virtual void create_constant(
		const char* name,
		Py::Callable& callback,
		Py::Object* user_data = 0
	);

	DOCSTRING(delete_constant, "Removes a constant from the virtual machine.")
	virtual void delete_constant(const char* name);

	DOCSTRING(get_variable, "Retrieves a variable from the virtual machine.")
	DOCSTRING_ARG(const char* name, "Name of the variable in the virtual machine")
	DOCSTRING_ARG(int max_len, "Maximum length of data to retrieve (required for binary values)")
	virtual Py::Object* get_variable(
		const char* name,
		int max_len=-1
	);

	// TODO add other options from 'unqlite_vm_config'
	DOCSTRING(set_vm_options, "Retrieves a variable from the virtual machine.")
	DOCSTRING_ARG(const char* import_path, "(Optional) Sets an additional import path for scripts")
	DOCSTRING_ARG(bool report_errors, "(Optional) Include errors in virtual machine output")
	DOCSTRING_ARG(int recursion_depth, "(Optional) Sets the function recursion limit when running the virtual machine")
	virtual void set_vm_options(
		const char* import_path = 0,
		bool report_errors = false,
		int recursion_depth = -1
	);

	// TODO extract_variable
	// TODO dump
	// TODO other options in 'unqlite_vm_config'

protected:

	// constants registry entry
	struct ConstantRecord
	{
		Py::Callable callback;
		Py::Object user_data;
	};
	typedef std::map<std::string, ConstantRecord*> ConstantMap;


	// TODO mark _db as a 'weak' references
	unqlite* _db;
	unqlite_vm* _vm;

	ConstantMap _constant_registry;

	// converters
	static int convert(const Py::Object& from, unqlite_value* to);
	static Py::Object* convert(unqlite_value* from, int max_len=-1);

	// static callbacks
	static void on_constant(unqlite_value *pValue, void *pUserData);
};

} // namespace pyunqlite

#endif // _UNQLITE_VIRTUALMACHINE_H
