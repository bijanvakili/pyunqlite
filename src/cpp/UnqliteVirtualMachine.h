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

	virtual void release();
	virtual void close();
	virtual void execute(
		pyunqlite::UserCallback* callback = 0,
		bool callback_as_binary = false
	);
	virtual void reset();

	virtual void create_constant(
		const char* name,
		Py::Callable& callback,
		Py::Object* user_data = 0
	);
	virtual void delete_constant(const char* name);

	// TODO add other options from 'unqlite_vm_config'
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

	// static callbacks
	static void on_constant(unqlite_value *pValue, void *pUserData);
};

} // namespace pyunqlite

#endif // _UNQLITE_VIRTUALMACHINE_H
