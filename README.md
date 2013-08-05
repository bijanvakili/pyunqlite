# pyunqlite

A python client library for unqlite embedded NoSQL database

## Building

### Requirements

1. python and development headers
2. SWIG
3. unqlite

### Building from source

This project is not yet using GNU Autotools.  For now, you need to use environment variables and the Makefile directly.

If you wish to keep third party libraries in a 'sandbox' environment, use the following environment variable:

    export THIRDPARTY_DIR=/path/to/my/sandbox

(NOTE: This is the 'prefix' directory that contains 'include', 'lib', etc.)

Then just run:

    make

## Execution

The wrapper and shared library will be found in the '.build' subfolder.

Until an 'install' target is created, you need to setup the following environment variables for the
shared library and python load paths:

    export LD_LIBRARY_PATH="$THIRDPARTY_DIR)/lib":`pwd`/.build
    export PYTHONPATH=`pwd`/src/python:`pwd`/.build

You can then run the test scripts as follows:

    cd src/python
    python test/test1.py
    . . .

## Testing Notes

Development and testing has so far taken place only Ubuntu Linux v12.04 LTS
