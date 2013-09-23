# pyunqlite

A python client library for the unqlite embedded NoSQL database

pyunqlite is licensed under the [MIT license](http://www.opensource.org/licenses/mit-license.php)

## Building

### Runtime Requirements

1. [python](http://www.python.org/) and development headers
2. [unqlite](http://unqlite.org/) as a static or shared library

### Build Requirements

1. [SWIG](http://www.swig.org/)
2. [setuptools](http://pythonhosted.org/setuptools/)

### Building from source

To build from source, run the following command:

	python setup.py build

## Installation

	python setup.py install

## Samples

After installation, you can run the test scripts as follows:

    cd src/python/examples
    python test/kv_intro.py
    (etc ...)

## Testing Notes

Development and testing has so far taken place only on Ubuntu Linux v12.04 LTS
