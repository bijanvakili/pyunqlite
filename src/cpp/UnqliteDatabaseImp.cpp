extern "C" {
#include "unqlite.h"
};
#include "UnqliteDatabaseImp.h"

namespace pyunqlite
{

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
UnqliteDatabaseImp::close()
{
	if (is_open())
	{
		unqlite_close(this->_db);
		this->_db = 0;
	}
}

bool
UnqliteDatabaseImp::is_open() const
{
	return (this->_db != 0);
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
