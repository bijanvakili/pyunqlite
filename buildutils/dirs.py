import os

class DirectoriesMixin:
    
    # Subdirectory to build unqlite library    
    _UNQLITE_SOURCE_SUBDIR = 'unqlite_source'

    # Wrapper source file directory
    _CPP_SOURCE_DIR=os.path.join('src', 'cpp')

    def _get_unqlite_build_dir(self):
        return os.path.join(self.build_temp, self._UNQLITE_SOURCE_SUBDIR)
