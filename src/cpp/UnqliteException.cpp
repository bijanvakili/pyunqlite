extern "C" {
#include "unqlite.h"
};
#include "UnqliteException.h"

namespace pyunqlite
{

UnqliteException::UnqliteException()
{
}

UnqliteException::UnqliteException(int error_code, unqlite* pDB)
{
	switch(error_code) {
	case UNQLITE_NOMEM:
		this->_error_text = "Out of memory";
		break;
	case UNQLITE_ABORT:
		this->_error_text = "Another thread have released this instance";
		break;
	case UNQLITE_IOERR:
		this->_error_text = "IO error";
		break;
	case UNQLITE_CORRUPT:
		this->_error_text = "Corrupt pointer";
		break;
	case UNQLITE_LOCKED:
		this->_error_text = "Forbidden Operation";
		break;
	case UNQLITE_BUSY:
		this->_error_text = "The database file is locked";
		break;
	case UNQLITE_DONE:
		this->_error_text = "Operation done";
		break;
	case UNQLITE_PERM:
		this->_error_text = "Permission error";
		break;
	case UNQLITE_NOTIMPLEMENTED:
		this->_error_text = "Method not implemented by the underlying Key/Value storage engine";
		break;
	case UNQLITE_NOTFOUND:
		this->_error_text = "No such record";
		break;
	case UNQLITE_NOOP:
		this->_error_text = "No such method";
		break;
	case UNQLITE_INVALID:
		this->_error_text = "Invalid parameter";
		break;
	case UNQLITE_EOF:
		this->_error_text = "End Of Input";
		break;
	case UNQLITE_UNKNOWN:
		this->_error_text = "Unknown configuration option";
		break;
	case UNQLITE_LIMIT:
		this->_error_text = "Database limit reached";
		break;
	case UNQLITE_EXISTS:
		this->_error_text ="Record exists";
		break;
	case UNQLITE_EMPTY:
		this->_error_text = "Empty record";
		break;
	case UNQLITE_COMPILE_ERR:
		this->_error_text = "Compilation error";
		break;
	case UNQLITE_VM_ERR:
		this->_error_text = "Virtual machine error";
		break;
	case UNQLITE_FULL:
		this->_error_text = "Full database";
		break;
	case UNQLITE_CANTOPEN:
		this->_error_text = "Unable to open the database file";
		break;
	case UNQLITE_READ_ONLY:
		this->_error_text = "Read only Key/Value storage engine";
		break;
	case UNQLITE_LOCKERR:
		this->_error_text = "Locking protocol error";
		break;
	default:
		this->_error_text = "Unknown Error";
		break;
	}

	AppendLogError(pDB);
}

UnqliteException::UnqliteException(const std::string& error_text, unqlite* pDB)
{
	this->_error_text = error_text;
	AppendLogError(pDB);
}

UnqliteException::~UnqliteException() throw()
{
}

const char*
UnqliteException::what() const throw()
{
	return this->_error_text.c_str();
}

void
UnqliteException::AppendLogError(unqlite* pDB)
{
	if (pDB)
	{
		int error_text_len = 0;
		int rc = unqlite_config(pDB, UNQLITE_CONFIG_ERR_LOG, 0, &error_text_len);
		if (rc == UNQLITE_OK)
		{
			std::string log_message(error_text_len + 1, '\0');

			rc = unqlite_config(pDB, UNQLITE_CONFIG_ERR_LOG, &log_message[0], &error_text_len);
			if (rc == UNQLITE_OK)
			{
				this->_error_text += '\n';
				this->_error_text += log_message;
			}
		}
	}
}


} // namespace pyunqlite
