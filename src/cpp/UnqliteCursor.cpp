extern "C" {
#include "unqlite.h"
};
#include "UnqliteException.h"
#include "UnqliteCursor.h"

namespace pyunqlite
{

UnqliteCursor::UnqliteCursor()
{
}

UnqliteCursor::UnqliteCursor(unqlite* db)
{
	int rc = unqlite_kv_cursor_init(db, &this->_cursor);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, db);

	this->_db = db;
	seek_begin();
}

UnqliteCursor::~UnqliteCursor()
{
	int rc = unqlite_kv_cursor_release(this->_db, this->_cursor);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

void
UnqliteCursor::seek(
	const char* key,
	int key_len,
	SeekMatchType match_type
)
{
	int iPos = 0;
	switch (match_type)
	{
	case SEEK_MATCH_EXACT:
		iPos = UNQLITE_CURSOR_MATCH_EXACT;
		break;
	case SEEK_MATCH_LESS_THAN:
		iPos = UNQLITE_CURSOR_MATCH_LE;
		break;
	case SEEK_MATCH_GREATER_THAN:
		iPos = UNQLITE_CURSOR_MATCH_GE;
		break;
	default:
		throw UnqliteException(UNQLITE_INVALID);
	}

	int rc = unqlite_kv_cursor_seek(this->_cursor, key, key_len, iPos);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}


void
UnqliteCursor::seek_begin()
{
	int rc = unqlite_kv_cursor_first_entry(this->_cursor);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

void
UnqliteCursor::seek_end()
{
	int rc = unqlite_kv_cursor_last_entry(this->_cursor);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

ValueBuffer*
UnqliteCursor::get_key(int key_len)
{
	int nBytes = 0;
	unqlite_kv_cursor_key(this->_cursor, 0, &nBytes);

	// create the buffer
	ValueBuffer* value = new ValueBuffer(false, nBytes);
	if (!value)
		throw UnqliteException(UNQLITE_NOMEM);

	int rc = unqlite_kv_cursor_key(this->_cursor, value->get_data(), &nBytes);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);

	return value;
}

ValueBuffer*
UnqliteCursor::get_data(
	bool as_binary,
	sxi64 value_len
)
{
	if (value_len < 0)
		value_len = get_data_len();

	// create the buffer
	ValueBuffer* value = new ValueBuffer(as_binary, value_len);
	if (!value)
		throw UnqliteException(UNQLITE_NOMEM);


	int rc = unqlite_kv_cursor_data(this->_cursor, value->get_data(), &value_len);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);

	return value;
}

sxi64
UnqliteCursor::get_data_len()
{
	sxi64 nBytes = 0;
	unqlite_kv_cursor_data(this->_cursor, 0, &nBytes);

	return nBytes;
}

void
UnqliteCursor::next()
{
	int rc = unqlite_kv_cursor_next_entry(this->_cursor);
	if (rc != UNQLITE_OK  && rc != UNQLITE_DONE)
		throw UnqliteException(rc, this->_db);
}

void
UnqliteCursor::prev()
{
	int rc = unqlite_kv_cursor_prev_entry(this->_cursor);
	if (rc != UNQLITE_OK  && rc != UNQLITE_DONE)
		throw UnqliteException(rc, this->_db);
}

bool
UnqliteCursor::is_valid() const
{
	return (bool)unqlite_kv_cursor_valid_entry(this->_cursor);

}

void
UnqliteCursor::remove()
{
	int rc = unqlite_kv_cursor_delete_entry(this->_cursor);
	if (rc != UNQLITE_OK)
		throw UnqliteException(rc, this->_db);
}

} // namespace pyunqlite
