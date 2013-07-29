extern "C" {
#include "unqlite.h"
};
#include "UnqliteException.h"
#include "ValueBuffer.h"

namespace pyunqlite {

ValueBuffer::ValueBuffer(bool is_binary, sxi64 data_len, char* data)
{
	this->_is_binary = is_binary;
	this->_data_len = data_len;
	this->_data = data;
	this->_is_allocated = false;
}

ValueBuffer::ValueBuffer(bool is_binary, sxi64 data_len)
{
	this->_is_binary = is_binary;
	this->_data_len = data_len;
	this->_data = new char [data_len + (is_binary ? 0 : 1)];
	if (!this->_data)
		throw UnqliteException(UNQLITE_NOMEM);
	if (!is_binary)
		this->_data[data_len] = 0;
}

ValueBuffer::~ValueBuffer()
{
	if (this->_is_allocated)
		delete this->_data;
}

bool
ValueBuffer::is_binary() const
{
	return this->_is_binary;
}

char*
ValueBuffer::get_data() const
{
	return this->_data;
}

sxi64
ValueBuffer::get_data_len() const
{
	return this->_data_len;
}

ValueBuffer::ValueBuffer()
{
}

ValueBuffer::ValueBuffer(const ValueBuffer&)
{
}

} // namespace pyunqlite
