extern "C" {
#include "unqlite.h"
};
#include "pyunqliteimp.h"

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

UnqliteCursor::UnqliteCursor()
{
}

UnqliteCursor::UnqliteCursor(unqlite* db)
{
	int rc = unqlite_kv_cursor_init(db, &this->_cursor);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, db);

	this->_db = db;

	rc = unqlite_kv_cursor_first_entry(this->_cursor);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, db);
}

UnqliteCursor::~UnqliteCursor()
{
	int rc = unqlite_kv_cursor_release(this->_db, this->_cursor);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

std::string
UnqliteCursor::get_key()
{
	int nBytes = 0;
	unqlite_kv_cursor_key(this->_cursor, 0, &nBytes);

	std::string key(nBytes + 1, '\0');
	unqlite_kv_cursor_key(this->_cursor, &key[0], &nBytes);

	return key;
}

std::string
UnqliteCursor::get_data()
{
	unqlite_int64 nBytes = 0;
	unqlite_kv_cursor_data(this->_cursor, 0, &nBytes);

	std::string data(nBytes + 1, '\0');
	unqlite_kv_cursor_data(this->_cursor, &data[0], &nBytes);

	return data;
}

void
UnqliteCursor::next()
{
	int rc = unqlite_kv_cursor_next_entry(this->_cursor);
	if (rc != UNQLITE_OK  && rc != UNQLITE_DONE)
		throw UnqliteException(rc, this->_db);
}

bool
UnqliteCursor::is_valid() const
{
	return (bool)unqlite_kv_cursor_valid_entry(this->_cursor);

}

UnqliteDatabaseImp::UnqliteDatabaseImp(
	const std::string& filename,
	bool create,
	bool read_only,
	bool temporary,
    bool use_journaling,
    bool use_mutex
)
{
	int mode_flags = 0;
	if (create)
	{
		mode_flags |= UNQLITE_OPEN_CREATE;
	}
	else
	{
		if (read_only)
			mode_flags |= UNQLITE_OPEN_READONLY;
		else
			mode_flags |= UNQLITE_OPEN_READWRITE;
	}
	if (temporary)
		mode_flags |= UNQLITE_OPEN_TEMP_DB;
	if (!use_journaling)
		mode_flags |= UNQLITE_OPEN_OMIT_JOURNALING;
	if (!use_mutex)
		mode_flags |= UNQLITE_OPEN_NOMUTEX;

	int rc = unqlite_open(&(this->_db), filename.c_str(), mode_flags);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc);
}

UnqliteDatabaseImp::UnqliteDatabaseImp()
{
}

UnqliteDatabaseImp::UnqliteDatabaseImp(const UnqliteDatabaseImp& )
{

}

UnqliteDatabaseImp::~UnqliteDatabaseImp()
{
	unqlite_close(this->_db);
}

void
UnqliteDatabaseImp::kv_store(
	const std::string& key,
	const std::string& value,
	bool append
)
{
	int rc;
	unqlite_int64 dataLen = value.length();
	if (append)
		rc = unqlite_kv_append(this->_db, key.c_str(), -1, value.c_str(), dataLen);
	else
		rc = unqlite_kv_store(this->_db, key.c_str(), -1, value.c_str(), dataLen);

	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

void
UnqliteDatabaseImp::kv_fetch(
	const std::string& key,
	std::string& value
)
{
	unqlite_int64 dataLen = 0;
	int rc = unqlite_kv_fetch(this->_db, key.c_str(), -1, 0, &dataLen);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);

	value.resize(dataLen);
	rc = unqlite_kv_fetch(this->_db, key.c_str(), -1, &value[0], &dataLen);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

void
UnqliteDatabaseImp::kv_delete(
	const std::string& key
)
{
	int rc = unqlite_kv_delete(this->_db, key.c_str(), -1);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

UnqliteCursor*
UnqliteDatabaseImp::kv_cursor()
{
	return new UnqliteCursor(this->_db);
}

unsigned int
UnqliteDatabaseImp::util_random_int()
{
	return unqlite_util_random_num(this->_db);
}

std::string
UnqliteDatabaseImp::util_random_string(unsigned int len)
{
	std::string str(len + 1, '\0');
	int rc = unqlite_util_random_string(this->_db, &str[0], len);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);

	return str;
}

} // namespace pyunqlite
