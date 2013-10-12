# pyunqlite

A python client library for the UnQLite embedded NoSQL database

pyunqlite is licensed under the [MIT license](http://www.opensource.org/licenses/mit-license.php)

## Building

### Runtime Requirements

1. [python](http://www.python.org/) 2.7.x
2. [unqlite](http://unqlite.org/) 1.1.6

TODO This project does not yet support python 3.x

### Build Requirements

1. [python](http://www.python.org/) and development headers
2. [unqlite](http://unqlite.org/) and development headers
3. [SWIG](http://www.swig.org/)
4. [setuptools](http://pythonhosted.org/setuptools/)

Optional requirements include:
* [sphinx](http://sphinx-doc.org/) for generating HTML documentation

### Building from source

pyunqlite can link to the unqlite library (libunqlite) in 3 ways:

1. Specify web URL 'unqlite-download-url' to download and build as a static library (default)
2. Specify 'unqlite-version' and optionaly 'unqlite-release-date'.  
   You can also drop in an unqlite .zip archive directly.  
   (e.g. if ```--build-base``` is ```build```, extract the archive into ```build/unqlite_source```)
3. Don't set anything.  pyunqlite will instead search for a pre-built 'libunqlite' static or shared library using standard include and linker paths.

Edit 'setup.cfg' if you wish to customize integration with unqlite based on the options above.
You can also override these options at runtime (see ```python setup.py --help build``` for details) 

To build, run the following command:

	python setup.py build


## Installation

To install, run the following command:

	python setup.py install

## Documentation

If you have [sphinx](http://sphinx-doc.org/) installed, you can build the pyunqlite documentation with the following command:

	python setup.py build_sphinx

By default, the documentation will be generated under ```build/sphinx/html```.  Use the ```--build-dir``` option to specify a different path.

## Samples

After installation, you can run the test scripts as follows:

    cd src/python/examples
    python test/kv_intro.py
    (etc ...)

## Testing Notes

The project has been tested on the following platforms:

* Ubuntu Linux v12.04 LTS 64-bit with GNU gcc
* Mac OS X Mountain Lion 10.8 with LLVM clang
* Microsoft Windows 7 64-bit with Visual Studio 2012

