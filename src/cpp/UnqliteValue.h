#ifndef _UNQLITE_VALUE_H
#define _UNQLITE_VALUE_H

#include "UnqliteCommon.h"

namespace pyunqlite
{

enum ValueType
{
	VALUETYPE_INT = 0,
	VALUETYPE_FLOAT,
	VALUETYPE_BOOL,
	VALUETYPE_STRING,
	VALUETYPE_NULL,
	VALUETYPE_NUMERIC,
	VALUETYPE_CALLABLE,
	VALUETYPE_SCALAR,
	VALUETYPE_JSON_ARRAY,
	VALUETYPE_JSON_OBJECT,
	VALUETYPE_RESOURCE,
	VALUETYPE_EMPTY
};

class UnqliteValue
{
public:
	virtual ~AbstractValue();

protected:

	ValueType get_type() const;
	unqlite_value* to_value() const;
	PyObject* to_python_object() const;

private:
	UnqliteValue();
};

} //namespace pyunqlite

#endif // _UNQLITE_VALUE_H
