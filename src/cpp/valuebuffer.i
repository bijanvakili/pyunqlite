/*
 * ValueBuffer SWIG definitions
 */

 /* Encapsulate 'value' as a string or byte buffer */
%typemap(in) const pyunqlite::ValueBuffer& value {

	if (PyString_Check($input)) {
		$1 = new pyunqlite::ValueBuffer(false, PyString_Size($input), PyString_AsString($input));  
	}
	else if(PyByteArray_Check($input)) {
		$1 = new pyunqlite::ValueBuffer(true, PyByteArray_Size($input), PyByteArray_AsString($input));
	}
	else {
		PyErr_SetString(PyExc_ValueError, "Expecting a string or byte array");
		return 0;
	}
	
	if (!$1) {
		PyErr_SetString(PyExc_MemoryError, "Unable to allocate metadata for buffer");
		return 0;
	}
}

%typemap(freearg) const pyunqlite::ValueBuffer& value {
	delete $1;
}

%typemap(out) pyunqlite::ValueBuffer* {
	if ($1->is_binary()) {
		// TODO this should be allocated with a null buffer prior to calling the function to avoid a double copy
		$result = PyByteArray_FromStringAndSize(
			static_cast< const char* >($1->get_data()),
			$1->get_data_len()
		);
	}
	else {
		$result = PyString_FromStringAndSize(
			static_cast< const char* >($1->get_data()),
			$1->get_data_len()
		);
	}
	
	delete $1;
}
