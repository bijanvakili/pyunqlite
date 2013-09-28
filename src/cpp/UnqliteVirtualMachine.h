#ifndef _UNQLITE_VIRTUALMACHINE_H
#define _UNQLITE_VIRTUALMACHINE_H

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

	// TODO mark _db as a 'weak' reference
	unqlite* _db;
	unqlite_vm* _vm;
};

} // namespace pyunqlite

#endif // _UNQLITE_VIRTUALMACHINE_H
