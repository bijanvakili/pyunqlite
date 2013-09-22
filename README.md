# pyunqlite

A python client library for the unqlite embedded NoSQL database

pyunqlite is licensed under the [MIT license](http://www.opensource.org/licenses/mit-license.php)

## Building

### Runtime Requirements

1. [python](http://www.python.org/) and development headers
2. [unqlite](http://unqlite.org/)

### Build Requirements

1. [SWIG](http://www.swig.org/)
2. [setuptools](http://pythonhosted.org/setuptools/)

### Building from source

	python setup.py build

## Execution

	python setup.py install

For now, you need to setup the following environment variables to load the python module

    export PYTHONPATH=`pwd`/src/cpp:`pwd`/src/python

You can then run the test scripts as follows:

    cd src/python/examples
    python test/kv_intro.py
    . . .

## Testing Notes

Development and testing has so far taken place only on Ubuntu Linux v12.04 LTS
