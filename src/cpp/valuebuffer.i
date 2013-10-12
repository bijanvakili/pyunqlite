/*
 * ValueBuffer SWIG definitions
 */

 /* Encapsulate 'value' as a string or byte buffer */
%typemap(in) const pyunqlite::ValueBuffer& value {
	$1 = new pyunqlite::ValueBuffer($input);  
	if (!$1) {
		PyErr_SetString(PyExc_MemoryError, "Unable to allocate metadata for buffer");
		return 0;
	}
}

%typemap(freearg) const pyunqlite::ValueBuffer& value {
	delete $1;
}

%typemap("doc") const pyunqlite::ValueBuffer& value "$1_name: Data value"

%typemap(in) pyunqlite::ValueBuffer* direct_buffer {
	$1 = new pyunqlite::ValueBuffer($input);  
	if (!$1) {
		PyErr_SetString(PyExc_MemoryError, "Unable to allocate metadata for buffer");
		return 0;
	}
}

%typemap(freearg) pyunqlite::ValueBuffer* direct_buffer {
	delete $1;
}


%typemap(out) pyunqlite::ValueBuffer* {
	if ($1) {
		$result = $1->get_python_object();
		delete $1;
	}
	else {
		$result = SWIG_Py_Void();
	}
}

%typemap("doc") pyunqlite::ValueBuffer* direct_buffer "$1_name: Optional buffer to directly store data"
