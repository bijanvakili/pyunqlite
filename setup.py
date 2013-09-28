from setuptools import setup, Extension
import glob
import os

source_files=glob.glob(os.path.join('src', 'cpp', '*.cpp'))
source_files.extend(glob.glob(os.path.join('src', 'cpp', 'pyunqliteimp.i')))
header_files=glob.glob(os.path.join('src', 'cpp', '*.h'))
header_files.extend(glob.glob(os.path.join('src', 'cpp', 'usercallback.i')))
header_files.extend(glob.glob(os.path.join('src', 'cpp', 'valuebuffer.i')))

setup(
    name='pyunqlite',
    version='0.0.1',
    description='python wrapper for unqlite',
    long_description='A python client library for the unqlite embedded NoSQL database',
    keywords=['UnQLite'],
    author='Bijan Vakili',
    author_email='bijvak2@gmail.com',
    url='https://github.com/bijanvakili/pyunqlite',
    package_dir = { '' : 'src/python', },
    packages = ['pyunqlite'],
    license='MIT License',
    platforms=['unix'],
    ext_modules=[Extension(name='_pyunqliteimp',
                          sources=source_files,
                          depends=header_files,
                          language='c++',
                          swig_opts=['-c++', '-builtin', '-keyword'],
                          libraries=['unqlite'])],
)

# TODO add 'test_suite'
# TODO extend clean target to actually remove wrapper code 
#     (see python bug #2624: http://bugs.python.org/issue2624)
# TODO support python 3 using --> use_2to3 = True,
# TODO exclude -Wmissing-prototypes on Linux
