#include "UnqliteCommon.h"
#include "UnqliteException.h"
#include "UnqliteCursor.h"

namespace pyunqlite
{


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

	virtual void close();
	virtual bool is_open() const;

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
