#ifndef _UNQLITE_BUFFER_H
#define _UNQLITE_BUFFER_H

#include "UnqliteCommon.h"

namespace pyunqlite
{

// encapsulates a string or byte buffer
class ValueBuffer
{
public:
	ValueBuffer(bool is_binary, sxi64 data_len, char* data);
	ValueBuffer(bool is_binary, sxi64 data_len);
	virtual ~ValueBuffer();

	bool is_binary() const;
	char* get_data() const;
	sxi64 get_data_len() const;

protected:
	bool _is_allocated;
	bool _is_binary;
	char* _data;
	sxi64 _data_len;

private:
	// disallowed
	ValueBuffer();
	ValueBuffer(const ValueBuffer&);
};

} //namespace pyunqlite

#endif // _UNQLITE_BUFFER_H
