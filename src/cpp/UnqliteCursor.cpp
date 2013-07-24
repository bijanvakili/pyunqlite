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

} // namespace pyunqlite
