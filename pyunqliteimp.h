#ifndef _PYUNQLITE_H
#define _PYUNQLITE_H

#include <exception>
#include <string>

// forward declarations
extern "C" {
typedef struct unqlite unqlite;
}; // extern "C"

namespace pyunqlite {

// common exception class
class UnqliteException: public std::exception
{
public:
	UnqliteException(int error_code, unqlite* pDB = 0);
	UnqliteException(const std::string& error_text, unqlite* pDB = 0);
	virtual ~UnqliteException() throw();

	virtual const char* what() const throw();

protected:
	std::string _error_text;

	virtual void AppendLogError(unqlite* pDB = 0);

private:
	UnqliteException();
};

class UnqliteCursor
{
public:
	UnqliteCursor(unqlite* db);
	virtual ~UnqliteCursor();

	void get_key(std::string& key);
	void get_data(std::string& data);
	void next();
	bool is_valid() const;

protected:
	unqlite* _db;
	unqlite_kv_cursor* _cursor;

private:
	UnqliteCursor();
};

// main database class
class UnqliteDatabaseImp
{
public:
	UnqliteDatabaseImp(
		const std::string& filename,
		bool create,
		bool read_only,
		bool temporary,
        bool use_journaling,
        bool use_mutex
    );
	virtual ~UnqliteDatabaseImp();

	virtual void kv_store(
		const std::string& key,
		const std::string& value,
		bool append
	);

    virtual void kv_fetch(
    	const std::string& key,
    	std::string& value
    );

    virtual void kv_delete(
    	const std::string& key
    );

    virtual UnqliteCursor kv_cursor();

    virtual unsigned int util_random_int();
    virtual std::string util_random_string(unsigned int len);

protected:
    unqlite* _db;

private:
    // disallowed
	UnqliteDatabaseImp();
    UnqliteDatabaseImp(const UnqliteDatabaseImp& );
};

} // pyunqlite

#endif // _PYUNQLITE_H
