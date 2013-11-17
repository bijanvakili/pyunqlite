#ifndef _UNQLITE_CURSOR_H
#define _UNQLITE_CURSOR_H

#include "UnqliteCommon.h"
#include "Callback.h"
#include "ValueBuffer.h"

namespace pyunqlite
{

// search type
enum SeekMatchType {
	SEEK_MATCH_EXACT = 0,
	SEEK_MATCH_LE = -1,
	SEEK_MATCH_GE = 1
};

// key-value cursor
class UnqliteCursor
{
public:
	UnqliteCursor(unqlite* db);
	virtual ~UnqliteCursor();

	DOCSTRING(close, "Explicitly closes the cursor.")
	virtual void close();

	DOCSTRING(get_key, "Retrieves the key at the current cursor position.")
	DOCSTRING_ARG(int key_len, "Optional maximum number of bytes to read from 'key'")
	virtual pyunqlite::ValueBuffer* get_key(
		int key_len=-1,
		pyunqlite::UserCallback* callback=0
	);

	DOCSTRING(get_key_len, "Retrieves the key length at the current cursor position.")
	virtual int get_key_len();

	DOCSTRING(get_data, "Retrieves the data at the current cursor position.")
	DOCSTRING_ARG(bool as_binary, "If set to 'true', data will be retrieved as a 'bytearray'")
	DOCSTRING_ARG(sxi64 value_len, "Optional data length")
	DOCSTRING_ARG(pyunqlite::UserCallback* callback, "Optional data consumer callback")
	DOCSTRING_ARG(pyunqlite::ValueBuffer* direct_buffer, "Optional buffer reference to directly load data")
	virtual pyunqlite::ValueBuffer* get_data(
		bool as_binary=false,
		sxi64 value_len=-1,
		pyunqlite::UserCallback* callback=0,
		pyunqlite::ValueBuffer* direct_buffer=0
	);

	DOCSTRING(get_data_len, "Retrieves the size of data at the current cursor position.")
	virtual sxi64 get_data_len();

	DOCSTRING(next, "Moves the cursor forward to the next item.")
	virtual void next();

	DOCSTRING(prev, "Moves the cursor backwards to the previous item.")
	virtual void prev();

	DOCSTRING(seek, "Moves the cursor to a position specified by a search key.")
	DOCSTRING_ARG(const char* key, "Record key")
	DOCSTRING_ARG(int key_len, "Optional maximum number of bytes to read from 'key'")
	DOCSTRING_ARG(SeekMatchType match_type, "Optional key search type")
	virtual void seek(
		const char* key,
		int key_len=-1,
		SeekMatchType match_type=SEEK_MATCH_EXACT
	);

	DOCSTRING(seek_begin, "Moves the cursor to the first item in the database.")
	virtual void seek_begin();

	DOCSTRING(seek_end, "Moves the cursor to the last item in the database.")
	virtual void seek_end();

	DOCSTRING(is_valid, "Determines if the cursor is at a valid position.")
	virtual bool is_valid() const;

	DOCSTRING(remove, "Removes the item at the current database position.")
	virtual void remove();

protected:
	// TODO mark _db as a 'weak' reference
	unqlite* _db;
	unqlite_kv_cursor* _cursor;

private:
	UnqliteCursor();
};


} // namespace pyunqlite

#endif // _UNQLITE_CURSOR_H

