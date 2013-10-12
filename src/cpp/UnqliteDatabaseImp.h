#ifndef _UNQLITE_DATABASEIMP_H
#define _UNQLITE_DATABASEIMP_H

#include "UnqliteCommon.h"
#include "UnqliteException.h"
#include "Callback.h"
#include "UnqliteVirtualMachine.h"
#include "ValueBuffer.h"

namespace pyunqlite
{

class UnqliteCursor;
class UnqliteVirtualMachine;

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

	DOCSTRING(close, "Close the database")
	virtual void close();

	DOCSTRING(is_open, "Determines if the database handle is still open")
	virtual bool is_open() const;

	DOCSTRING(set_db_options, "Sets the database options")
	DOCSTRING_ARG(int max_cache_pages, "Maximum raw pages to cache in memory")
	DOCSTRING_ARG(bool auto_commit, "If set to 'false', commit() must be called to commit all database changes")
	virtual void set_db_options(
		int max_cache_pages = 0,
		bool auto_commit = true
	);

	DOCSTRING(get_kv_engine, "Extract the name of the underlying Key/Value storage engine (i.e. Hash, Mem, R+Tree, LSM, etc.)")
	virtual std::string get_kv_engine() const;

	DOCSTRING(kv_store, "Stores records in the database")
	DOCSTRING_ARG(const char* key, "Record key")
	DOCSTRING_ARG(sxi64 value_len, "Optional data length")
	DOCSTRING_ARG(int key_len, "Optional maximum number of bytes to read from 'key'")
	DOCSTRING_ARG(bool append, "If set to 'true', data will be appended to the existing key")
	virtual void kv_store(
		const char* key,
		const pyunqlite::ValueBuffer& value,
		sxi64 value_len=-1,
		int key_len=-1,
		bool append=false
	);

	DOCSTRING(kv_fetch, "Fetch a record from the database")
	DOCSTRING_ARG(bool as_binary, "If set to 'true', data will be retrieved as a 'bytearray'")
    virtual pyunqlite::ValueBuffer* kv_fetch(
    	const char* key,
    	bool as_binary=false,
    	int key_len=-1,
    	sxi64 value_len=-1,
    	pyunqlite::UserCallback* callback=0,
    	pyunqlite::ValueBuffer* direct_buffer=0
    );

	DOCSTRING(kv_fetch_len, "Fetch the length a of record's data from the database")
    virtual sxi64 kv_fetch_len(
		const char* key,
		int key_len=-1
    );

	DOCSTRING(kv_delete, "Remove a record from the database")
    virtual void kv_delete(
    	const char* key,
    	int key_len=-1
    );

	DOCSTRING(kv_exists, "Determines if a key exists in the table")
    virtual bool kv_exists(
		const char* key,
		int key_len=-1
    );

	DOCSTRING(kv_cursor, "Creates a database cursor for iteration")
    virtual UnqliteCursor* kv_cursor();

	DOCSTRING(start_transaction, "Manually begin a write-transaction on the specified database handle")
    virtual void start_transaction();

	DOCSTRING(rollback, "Rollback a write-transaction on the specified database handle")
    virtual void rollback();

	DOCSTRING(commit, "Commit all changes to the database")
    virtual void commit();

	DOCSTRING(util_random_int, "Generate random number using the UnQLite PRNG")
    virtual unsigned int util_random_int();

	DOCSTRING(util_random_string, "Generate random string using the UnQLite PRNG")
	DOCSTRING_ARG(unsigned int len, "Length of the random string")
    virtual std::string util_random_string(unsigned int len);

    virtual UnqliteVirtualMachine*
    vm_compile(
        const char* filename = 0,
    	const char* jx9_content = 0,
    	int jx9_content_len = -1
    );

protected:
    unqlite* _db;

private:
    // disallowed
	UnqliteDatabaseImp();
    UnqliteDatabaseImp(const UnqliteDatabaseImp& );
};

} // namespace pyunqlite

#endif // _UNQLITE_DATABASEIMP_H
