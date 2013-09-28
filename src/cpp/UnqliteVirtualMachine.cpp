#include <string.h>
extern "C" {
#include "unqlite.h"
};

#include "UnqliteVirtualMachine.h"

namespace pyunqlite
{

UnqliteVirtualMachine::UnqliteVirtualMachine(unqlite* db, unqlite_vm* vm)
{
	this->_db = db;
	this->_vm = vm;
}

UnqliteVirtualMachine::~UnqliteVirtualMachine()
{
	release();
}

void
UnqliteVirtualMachine::release()
{
	if (this->_vm)
	{
		unqlite_vm_release( this->_vm );
		this->_vm = 0;
	}
}

void
UnqliteVirtualMachine::close()
{
	release();
}

void
UnqliteVirtualMachine::execute(
	pyunqlite::UserCallback* callback,
	bool callback_as_binary
)
{
	int rc;
	if (callback)
	{
		callback->set_as_binary(callback_as_binary);
		rc = unqlite_vm_config(
			this->_vm,
			UNQLITE_VM_CONFIG_OUTPUT,
			callback->get_unqlite_callback_function(),
			callback->get_unqlite_callback_data()
		);
		if ( rc != 0 )
			throw UnqliteException(rc, this->_db);
	}

	rc = unqlite_vm_exec( this->_vm );
	if ( rc != 0 )
		throw UnqliteException(rc, this->_db);
}

void
UnqliteVirtualMachine::reset()
{
	int rc = unqlite_vm_reset( this->_vm );
	if ( rc != 0 )
		throw UnqliteException(rc, this->_db);
}

void
UnqliteVirtualMachine::set_vm_options(
	const char* import_path,
	bool report_errors,
	int recursion_depth
)
{
	int rc;

	if (import_path)
	{
		rc = unqlite_vm_config(
			this->_vm,
			UNQLITE_VM_CONFIG_IMPORT_PATH,
			import_path
		);
		if ( rc != 0 )
			throw UnqliteException(rc, this->_db);
	}

	if (report_errors)
	{
		rc = unqlite_vm_config(
			this->_vm,
			UNQLITE_VM_CONFIG_ERR_REPORT
		);
		if ( rc != 0 )
			throw UnqliteException(rc, this->_db);
	}

	if (recursion_depth >= 0)
	{
		rc = unqlite_vm_config(
			this->_vm,
			UNQLITE_VM_CONFIG_RECURSION_DEPTH,
			recursion_depth
		);
		if ( rc != 0 )
			throw UnqliteException(rc, this->_db);
	}
}


} // namespace pyunqlite
