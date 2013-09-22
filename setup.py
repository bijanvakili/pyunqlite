from setuptools import setup, Extension
import glob
import os

cpp_files = glob.glob(os.path.join('src', 'cpp', '*.cpp'))
cpp_files.extend(glob.glob(os.path.join('src', 'cpp', 'pyunqliteimp.i')))

setup(
    name='pyunqlite',
    version='0.0.1',
    description='python wrapper for unqlite',
    author='Bijan Vakili',
    author_email='bijanvakili@server.fake',
    url='https://github.com/bijanvakili/pyunqlite',
    package_dir = { '' : 'src/python', },
    packages = ['pyunqlite'],
    license='MIT',
    ext_modules=[Extension(name='_pyunqliteimp',
                          sources=cpp_files, 
                          language='c++',
                          swig_opts=['-c++', '-builtin', '-keyword'],
                          libraries=['unqlite'])],
)

# TODO extend clean target to actually remove wrapper code

# TODO add 'test_suite'
# TODO support python 3 using --> use_2to3 = True,
# TODO exclude -Wmissing-prototypes on Linux
