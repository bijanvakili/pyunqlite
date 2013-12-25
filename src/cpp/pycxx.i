/*
 * PyCXX SWIG definitions
 */

%typemap(in) Py::Callable& callback {
 	$1 = new Py::Callable($input);
	if (!$1) {
		PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for callback");
		return 0;
	}
}
%typemap(freearg) Py::Callable& callback {
	delete $1;
}
%typemap("doc") Py::Callable& callback "$1_name: User-defined callback"

%typemap(in) Py::Object* user_data {
	$1 = new Py::Object($input);
	if (!$1) {
		PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for user data");
		return 0;
	}
}
%typemap(freearg) Py::Object* user_data {
	delete $1;
}
%typemap("doc") Py::Object* user_data "$1_name: User-defined data"

%typemap(out) Py::Object* {
	if ($1) {
		$result = $1->ptr();
		$1->increment_reference_count();
		delete $1;
	}
	else {
		$result = SWIG_Py_Void();
	}
}
