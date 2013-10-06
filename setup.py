from setuptools import setup
from buildutils.clean import CleanCommand
from buildutils.libunqlite import BuildUnqliteCommand 
from buildutils.pyunqliteimp import BuildPyUnliteImpCommand
from buildutils.install_lib import InstallLibCommand
from setuptools import Extension


def main():

    setup(
        name='pyunqlite',
        version='0.0.1',
        description='python wrapper for unqlite',
        long_description='A python client library for the unqlite embedded NoSQL database',
        keywords=['UnQLite'],
        author='Bijan Vakili',
        author_email='bijvak2@gmail.com',
        url='https://github.com/bijanvakili/pyunqlite',
        cmdclass={'build_clib' : BuildUnqliteCommand,
                  'build_ext': BuildPyUnliteImpCommand,
                  'install_lib': InstallLibCommand,
                  'clean' : CleanCommand },
        package_dir = { '' : 'src/python', },
        packages = ['pyunqlite'],
        license='MIT License',
        
        # details to be filled in by subcommands
        libraries=[('unqlite', { })],
        ext_modules=[Extension(
                               name='_pyunqliteimp',
                               language='c++',
                               swig_opts=['-c++', '-builtin', '-keyword'],
                               libraries=['unqlite'],
                               sources=[ ])],     
    )

if __name__ == "__main__":
    main()

# TODO add 'test_suite'
# TODO support python 3 using --> use_2to3 = True,
