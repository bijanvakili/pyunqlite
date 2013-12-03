extern "C" {
#include "unqlite.h"
};
#include "CXX/Objects.hxx"
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
	for (ConstantMap::iterator it = _constant_registry.begin(); it != _constant_registry.end(); ++it)
		delete it->second;
	_constant_registry.clear();

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
UnqliteVirtualMachine::create_constant(
	const char* name,
	Py::Callable& callback,
	Py::Object* user_data
)
{
	// register the callback and user data
	ConstantRecord* pRecord = new ConstantRecord();
	if (!pRecord)
		throw UnqliteException(UNQLITE_NOMEM, this->_db);
	pRecord->callback = Py::Callable(callback);
	if (user_data)
		pRecord->user_data = Py::Object(*user_data);
	else
		pRecord->user_data = Py::Object(Py_None);

	_constant_registry[name] = pRecord;

	// insert the callback into the virtual machine
	int rc = unqlite_create_constant(this->_vm, name, on_constant, pRecord);
	if (rc != UNQLITE_OK)
	{
		_constant_registry.erase(name);
		delete pRecord;
		throw UnqliteException(rc, this->_db);
	}
}

void
UnqliteVirtualMachine::delete_constant(const char* name)
{
	int rc = unqlite_delete_constant(this->_vm, name);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);

	ConstantMap::iterator it = this->_constant_registry.find(name);
	if ( it != this->_constant_registry.end() )
	{
		delete it->second;
		this->_constant_registry.erase(it);
	}
}


void
UnqliteVirtualMachine::on_constant(unqlite_value *pValue, void *pUserData)
{
	ConstantRecord* pRecord = (ConstantRecord*)pUserData;

	// if it wasn't registered, then just return a null value
	if (!pRecord)
	{
		unqlite_value_null(pValue);
		return;
	}

	// execute python callback with user_data
	Py::Tuple args(1);
	args[0] = pRecord->user_data;
	Py::Object constant_result = pRecord->callback.apply(args);

	// convert and store result in pValue
	// TODO add error handling for conversion
	convert(constant_result, pValue);
}

int
UnqliteVirtualMachine::convert(const Py::Object& src, unqlite_value* dest)
{
	int rc;

	if (src.isNone())
	{
		rc = unqlite_value_null(dest);
	}
	else if (PyInt_Check(*src))
	{
		Py::Int srcInteger(src);
		if (PyInt_CheckExact(*src))
			rc = unqlite_value_int(dest, (long)srcInteger);
		else
			rc = unqlite_value_int64(dest, (unqlite_int64)srcInteger.asLongLong());
	}
	else if (src.isBoolean())
	{
		rc = unqlite_value_bool(dest, src.isTrue());
	}
	else if (PyFloat_Check(*src))
	{
		Py::Float srcFloat(src);
		rc = unqlite_value_double(dest, (double)srcFloat);
	}
	else if (src.isString())
	{
		Py::String srcStringObj(src);
		std::string srcString((std::string)srcStringObj);
		rc = unqlite_value_string(dest, srcString.c_str(), srcString.size());
	}
	else if (PyByteArray_Check(*src))
	{
		rc = unqlite_value_resource(dest, (void*)PyByteArray_AsString(*src));
	}
	else
	{
		unqlite_value_null(dest);
		rc = UNQLITE_INVALID;
	}

	return rc;
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
