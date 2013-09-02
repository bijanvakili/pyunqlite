/*
 * UserCallback SWIG definitions
 */

 %typemap(in) pyunqlite::UserCallback* callback {
	$1 = new pyunqlite::UserCallback($input);  
	if (!$1) {
		PyErr_SetString(PyExc_MemoryError, "Unable to allocate metadata for user callback");
		return 0;
	}
}

%typemap(freearg) pyunqlite::UserCallback* callback {
	delete $1;
}
