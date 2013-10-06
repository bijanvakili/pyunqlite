from distutils import log
from distutils.command.clean import clean as CleanBase
from distutils.dir_util import remove_tree
import os
from dirs import DirectoriesMixin  

class CleanCommand(CleanBase, DirectoriesMixin):
    
    def run(self):
        # remove the swig generated files
        # (due to python bug #2624: http://bugs.python.org/issue2624)
        pathname = os.path.join(self._CPP_SOURCE_DIR, 'pyunqliteimp.py')
        if os.path.exists(pathname) and not self.dry_run:
            os.remove(pathname)
        pathname = os.path.join(self._CPP_SOURCE_DIR, 'pyunqliteimp_wrap.cpp')
        if os.path.exists(pathname) and not self.dry_run:
            os.remove(pathname)
        
        # remove the unqlite download and source directory
        unqlite_src_dir = self._get_unqlite_build_dir()
        if os.path.exists(unqlite_src_dir):
            log.info('Removing ' + unqlite_src_dir)
            remove_tree(unqlite_src_dir, self.dry_run)
        else:
            log.info("'{0}' does not exist -- can't clean it".format(unqlite_src_dir))
                
        CleanBase.run(self)
