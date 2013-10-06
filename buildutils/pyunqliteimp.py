from distutils.command.build_ext import build_ext as BuildExtBase
import glob
import os
from dirs import DirectoriesMixin

# Only Linux, there will be annoying warnings due to '-Wmissing-prototypes' gcc 
# option being used for C++ compilation.
# 
# See:
#    http://stackoverflow.com/questions/8106258/cc1plus-warning-command-line-option-wstrict-prototypes-is-valid-for-ada-c-o
#    http://svn.python.org/view/python/trunk/configure.in?view=markup

class BuildPyUnliteImpCommand(BuildExtBase, DirectoriesMixin):
    
    def finalize_options(self):
        
        BuildExtBase.finalize_options(self)
        
        # collect all the source files for building
        cpp_src_dir = self._CPP_SOURCE_DIR
        source_files=glob.glob(os.path.join(cpp_src_dir, '*.cpp'))
        source_files.extend(glob.glob(os.path.join(cpp_src_dir, 'pyunqliteimp.i')))
        header_files=glob.glob(os.path.join(cpp_src_dir, '*.h'))
        header_files.extend(glob.glob(os.path.join(cpp_src_dir, 'usercallback.i')))
        header_files.extend(glob.glob(os.path.join(cpp_src_dir, 'valuebuffer.i')))

        include_dirs=[]
        
        # Use the headers from the unqlite build directory if they were extracted
        unqlite_src_dir = self._get_unqlite_build_dir()
        if os.path.exists(unqlite_src_dir):
            include_dirs.append(unqlite_src_dir) 

        # update source information
        for module in self.extensions:
            if module.name == '_pyunqliteimp':
                module.sources=source_files
                module.depends=header_files
                module.include_dirs=[]

    def run(self):
        # Use the headers from the unqlite build directory if they were extracted
        unqlite_src_dir = self._get_unqlite_build_dir()
        if os.path.exists(unqlite_src_dir):
            if not self.include_dirs:
                self.include_dirs=[]
            self.include_dirs.append(unqlite_src_dir)

        BuildExtBase.run(self)

