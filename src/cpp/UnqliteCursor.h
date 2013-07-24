#ifndef _UNQLITE_CURSOR_H
#define _UNQLITE_CURSOR_H

#include "UnqliteCommon.h"

namespace pyunqlite
{

class UnqliteCursor
{
public:
	UnqliteCursor(unqlite* db);
	virtual ~UnqliteCursor();

	std::string get_key();
	std::string get_data();
	void next();
	bool is_valid() const;

protected:
	unqlite* _db;
	unqlite_kv_cursor* _cursor;

private:
	UnqliteCursor();
};


} // namespace pyunqlite

#endif // _UNQLITE_CURSOR_H

