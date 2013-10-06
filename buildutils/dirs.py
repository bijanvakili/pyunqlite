import os

class DirectoriesMixin:
    
    # Subdirectory to build unqlite library    
    _UNQLITE_SOURCE_SUBDIR = 'unqlite_source'

    # Wrapper source file directory
    _CPP_SOURCE_DIR=os.path.join('src', 'cpp')

    def _get_unqlite_build_dir(self):
        if not hasattr(self, 'build_base') or not self.build_base:
            self.build_base = None
            self.set_undefined_options('build', 
                                       ('build_base', 'build_base'))
        return os.path.join(self.build_base, self._UNQLITE_SOURCE_SUBDIR)
