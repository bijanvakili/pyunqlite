# pyunqlite

A python client library for the unqlite embedded NoSQL database

pyunqlite is licensed under the [MIT license](http://www.opensource.org/licenses/mit-license.php)

## Building

### Requirements

1. [python](http://www.python.org/) and development headers
2. [SWIG](http://www.swig.org/)
3. [unqlite](http://unqlite.org/)

### Building from source

(TODO: Build scripts not using libtool not implemented yet)
Run the following:

    ./configure
    make


## Execution

The wrapper and shared library will be found in the '.build' subfolder.

Until an 'install' target is created, you need to setup the following environment variables for the
shared library and python load paths:

    export LD_LIBRARY_PATH=`pwd`/.build
    export PYTHONPATH=`pwd`/src/python:`pwd`/.build

You can then run the test scripts as follows:

    cd src/python
    python test/test1.py
    . . .

## Testing Notes

Development and testing has so far taken place only on Ubuntu Linux v12.04 LTS
