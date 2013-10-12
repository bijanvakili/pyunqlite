#include <string.h>
extern "C" {
#include "unqlite.h"
};

#include "UnqliteDatabaseImp.h"
#include "UnqliteCursor.h"

namespace pyunqlite
{

UnqliteDatabaseImp::UnqliteDatabaseImp(
	const char* filename,
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

	int rc = unqlite_open(&(this->_db), filename, mode_flags);
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
UnqliteDatabaseImp::set_db_options(
	int max_cache_pages,
	bool auto_commit
)
{
	int rc;
	if (max_cache_pages > 0 ) {
		rc = unqlite_config(this->_db, UNQLITE_CONFIG_MAX_PAGE_CACHE, max_cache_pages);
		if ( rc != 0 )
			throw UnqliteException(rc, this->_db);
	}
	if ( !auto_commit ) {
		rc = unqlite_config(this->_db, UNQLITE_CONFIG_DISABLE_AUTO_COMMIT);
		if ( rc != 0 )
			throw UnqliteException(rc, this->_db);
	}
}


std::string
UnqliteDatabaseImp::get_kv_engine() const
{
	char* pzName = 0;
	int rc = unqlite_config(this->_db, UNQLITE_CONFIG_GET_KV_NAME, &pzName);
	if ( rc != 0 )
		throw UnqliteException(rc, this->_db);

	return std::string(pzName);
}


void
UnqliteDatabaseImp::kv_store(
	const char* key,
	const ValueBuffer& value,
	sxi64 value_len,
	int key_len,
	bool append
)
{
	int rc;

	// for negative length, store the entire contents of the value
	if ( value_len < 0 )
		value_len = value.get_data_len();

	if (append)
		rc = unqlite_kv_append(this->_db, key, key_len, value.get_data(), value_len);
	else
		rc = unqlite_kv_store(this->_db, key, key_len, value.get_data(), value_len);

	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

ValueBuffer*
UnqliteDatabaseImp::kv_fetch(
	const char* key,
	bool as_binary,
	int key_len,
	sxi64 value_len,
	pyunqlite::UserCallback* callback,
	pyunqlite::ValueBuffer* direct_buffer
)
{
	// setup the buffer for retrieving data
	ValueBuffer* value = 0;
	if (direct_buffer) {

		// disallow a direct buffer if a callback was specified
		if (callback)
			throw UnqliteException(UNQLITE_INVALID);

		if (value_len < 0)
			value_len = direct_buffer->get_data_len();
		else if (direct_buffer->get_data_len() < value_len)
			throw UnqliteException(UNQLITE_INVALID);

		value = new ValueBuffer(*direct_buffer);
	}
	else if (!callback) {
		// determine the size of the stored data if it is unknown
		if (value_len < 0)
			value_len = kv_fetch_len(key, key_len);

		// create a new buffer
		value = new ValueBuffer(as_binary, value_len);
	}

	// retrieve the data
	int rc;
	if (callback) {
		callback->set_as_binary(as_binary);
		rc = unqlite_kv_fetch_callback(
			this->_db,
			key,
			key_len,
			callback->get_unqlite_callback_function(),
			callback->get_unqlite_callback_data()
		);
	}
	else {
		if (!value)
			throw UnqliteException(UNQLITE_NOMEM);

		rc = unqlite_kv_fetch(this->_db, key, key_len, value->get_data(), &value_len);
	}

	if (callback && (rc == UNQLITE_ABORT))
		callback->process_exception();
	else if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);

	return value;
}

sxi64
UnqliteDatabaseImp::kv_fetch_len(
	const char* key,
	int key_len
)
{
	unqlite_int64 data_len = 0;
	int rc = unqlite_kv_fetch(this->_db, key, key_len, 0, &data_len);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);

	return data_len;
}

// determines if a key exists in the table
bool
UnqliteDatabaseImp::kv_exists(
	const char* key,
	int key_len
)
{
	unqlite_int64 data_len = 0;
	int rc = unqlite_kv_fetch(this->_db, key, key_len, 0, &data_len);
	switch (rc)
	{
	case UNQLITE_OK:
		return true;
	case UNQLITE_NOTFOUND:
		return false;
	default:
		throw UnqliteException(rc, this->_db);
	}
}


void
UnqliteDatabaseImp::kv_delete(
	const char* key,
	int key_len
)
{
	int rc = unqlite_kv_delete(this->_db, key, key_len);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

UnqliteCursor*
UnqliteDatabaseImp::kv_cursor()
{
	return new UnqliteCursor(this->_db);
}


void
UnqliteDatabaseImp::start_transaction()
{
	int rc = unqlite_begin(this->_db);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}


void
UnqliteDatabaseImp::rollback()
{
	int rc = unqlite_rollback(this->_db);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}


void
UnqliteDatabaseImp::commit()
{
	int rc = unqlite_commit(this->_db);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
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

UnqliteVirtualMachine*
UnqliteDatabaseImp::vm_compile(
	const char* filename,
	const char* jx9_content,
	int jx9_content_len
)
{
	unqlite_vm* vm = 0;
	int rc;
	if (jx9_content)
	{
		rc = unqlite_compile(this->_db, jx9_content, jx9_content_len, &vm);
	}
	else if (filename)
	{
		rc = unqlite_compile_file(this->_db, filename, &vm);
	}
	else
	{
		throw UnqliteException(UNQLITE_INVALID, this->_db);
	}

	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);

	return new UnqliteVirtualMachine(this->_db, vm);
}


} // namespace pyunqlite
