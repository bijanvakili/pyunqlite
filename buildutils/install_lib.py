from distutils.command.install_lib import install_lib as InstallLibBase

"""
Fix for with bug in 'install_lib' command where 
'build_clib' is not being executed on clean 'install' due

         http://bugs.python.org/issue5243
"""
class InstallLibCommand(InstallLibBase):
    
    def build(self):
        if not self.skip_build:
            if self.distribution.has_c_libraries():
                self.run_command('build_clib')

        InstallLibBase.build(self)
