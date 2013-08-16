AC_PREREQ(2.61)

# TODO Retrieve version and email contact from outside this script
AC_INIT([pyunqlite], [0.01], [bijanvakili@server.make])

AC_ARG_WITH([swig],
	[AS_HELP_STRING([--with-swig], [specify swig executable (default to PATH)])],
	[with_swig=$withval],
	[with_swig=swig])

AC_MSG_NOTICE([Configuring pyunqlite])

# Checks for programs
AC_PROG_GREP
AC_PROG_CC
AC_PROG_CXXCPP
AC_PROG_CXX


# TODO Fix to use libtool for shared libraries (ensure -fPIC is included)
#lib_LTLIBRARIES = _pyunqliteimp.la
#AC_CONFIG_MACRO_DIR([m4])
#LT_INIT()
#AC_DISABLE_STATIC()
#AC_SUBST([LIBTOOL_DEPS])


# Checks for standard header files.
AC_HEADER_STDC
AC_CHECK_TYPES(long long)
AC_CHECK_HEADERS([exception])
AC_CHECK_HEADERS([float.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([math.h])
AC_CHECK_HEADERS([stdexcept])
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([string.h])

# Check for standard libraries
AC_CHECK_LIB([pthread], [pthread_create], [], AC_MSG_ERROR([pthread library not found.]), [])

# Check for swig v2.0
if test "$with_swig" = "swig"; then
	AC_CHECK_PROG(SWIGBIN, [swig], [swig], [noswig])
	if test "$SWIGBIN" = "noswig"; then
		AC_MSG_ERROR([swig not found])
	fi
else
	SWIGBIN=$with_swig
fi
swig_major_version=`$SWIGBIN -version 2>&1|grep 'SWIG Version'|cut -d" " -f3|cut -d"." -f1`
if [[ -z $swig_major_version ]]; then
	AC_MSG_ERROR([swig ($SWIGBIN) not found])
fi
if test ${swig_major_version} -lt 2; then
	AC_MSG_ERROR([Require swig to be at least version 2+])
fi
AC_SUBST(SWIGBIN)

# Check for python

AC_CHECK_HEADER([Python.h], [], AC_MSG_ERROR([python development headers not installed.]))

# Check unqlite

AC_CHECK_HEADER([unqlite.h], [], AC_MSG_ERROR([unqlite headers not found.]),
[[ #ifndef HAVE_UNQLITE_H
	#include <unqlite.h>
#endif
]])

AC_CHECK_LIB([unqlite], [unqlite_lib_ident], [], AC_MSG_ERROR([unqlite library not found.]), [-lpthread])

# Update files
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
