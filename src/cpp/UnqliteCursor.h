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
	SEEK_MATCH_LESS_THAN = -1,
	SEEK_MATCH_GREATER_THAN = 1
};

// key-value cursor
class UnqliteCursor
{
public:
	UnqliteCursor(unqlite* db);
	virtual ~UnqliteCursor();

	virtual pyunqlite::ValueBuffer* get_key(
		int key_len=-1,
		pyunqlite::UserCallback* callback=0
	);
	virtual pyunqlite::ValueBuffer* get_data(
		bool as_binary=false,
		sxi64 value_len=-1,
		pyunqlite::UserCallback* callback=0,
		pyunqlite::ValueBuffer* direct_buffer=0
	);
	virtual sxi64 get_data_len();

	virtual void next();
	virtual void prev();
	virtual void seek(
		const char* key,
		int key_len=-1,
		SeekMatchType match_type=SEEK_MATCH_EXACT
	);
	virtual void seek_begin();
	virtual void seek_end();
	virtual bool is_valid() const;

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

