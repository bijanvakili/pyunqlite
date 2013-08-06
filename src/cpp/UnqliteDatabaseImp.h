#ifndef _UNQLITE_DATABASEIMP_H
#define _UNQLITE_DATABASEIMP_H

#include "UnqliteCommon.h"
#include "UnqliteException.h"
#include "UnqliteCursor.h"
#include "ValueBuffer.h"

namespace pyunqlite
{


// main database class
class UnqliteDatabaseImp
{
public:
	UnqliteDatabaseImp(
		const char* filename=":mem:",
		bool create=true,
		bool read_only=false,
		bool temporary=false,
		bool use_journaling=true,
		bool use_mutex=true
    );
	virtual ~UnqliteDatabaseImp();

	virtual void close();
	virtual bool is_open() const;

	virtual void kv_store(
		const char* key,
		const pyunqlite::ValueBuffer& value,
		sxi64 value_len=-1,
		int key_len=-1,
		bool append=false
	);

	// returns the data
	// TODO kv_fetch() with callback
    virtual pyunqlite::ValueBuffer* kv_fetch(
    	const char* key,
    	bool as_binary=false,
    	int key_len=-1,
    	sxi64 value_len=-1,
    	pyunqlite::ValueBuffer* direct_buffer=0
    );

    // returns only the length of data within the buffer
    virtual sxi64 kv_fetch_len(
		const char* key,
		int key_len=-1
    );

    virtual void kv_delete(
    	const char* key,
    	int key_len=-1
    );

    virtual UnqliteCursor* kv_cursor();

    virtual unsigned int util_random_int();
    virtual std::string util_random_string(unsigned int len);

protected:
    unqlite* _db;

private:
    // disallowed
	UnqliteDatabaseImp();
    UnqliteDatabaseImp(const UnqliteDatabaseImp& );
};

} // namespace pyunqlite

#endif // _UNQLITE_DATABASEIMP_H
