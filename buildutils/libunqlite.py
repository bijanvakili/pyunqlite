from distutils import log
from distutils import ccompiler
from distutils.command.build_clib import build_clib as BuildCLibBase
import os
from urllib2 import urlopen, URLError, HTTPError
from urlparse import urlparse
from zipfile import ZipFile
from dirs import DirectoriesMixin

class BuildUnqliteCommand(BuildCLibBase, DirectoriesMixin):
    _UNQLITE_BASE_URL = "http://unqlite.org/db/"

    user_options = BuildCLibBase.user_options + [
                    ('unqlite-url', None, 'Download URL for unqlite source'),
                    ('unqlite-version=', None, 'unqlite library version (e.g. 1.1.6)'),
                    ('unqlite-release-date=', None, 'unqlite version release date (e.g. 2013-08-25)')]


    def initialize_options(self):
        self.unqlite_url = None
        self.unqlite_version = None
        self.unqlite_release_date = None
        
        self._unqlite_download_url = None
        self._unqlite_archive_filename = None
        self._dest_folder = None
        
        BuildCLibBase.initialize_options(self)


    def finalize_options(self):

        BuildCLibBase.finalize_options(self)
        
        self._dest_folder = self._get_unqlite_build_dir()
        
        # Option 1: User specified a download URL
        if self.unqlite_url:
            self._unqlite_download_url = self.unqlite_url
            self._unqlite_archive_filename = \
                os.path.basename(urlparse(self._unqlite_download_url).path)
                
        # Option 2: User specified at least a version
        elif self.unqlite_version:
            self._unqlite_archive_filename = 'unqlite-db-'
            if self.unqlite_release_date:
                self._unqlite_archive_filename += \
                    '{0}-'.format(self.unqlite_release_date.translate(None, '-'))
            self._unqlite_archive_filename += self.unqlite_version.translate(None, '.')
            self._unqlite_download_url = self._UNQLITE_BASE_URL + self._unqlite_archive_filename 
        
        # Option 3: Assume a libunqlite has been pre-built and is accessible for the
        #           'build_ext' step using include and linker paths
        if not self._unqlite_archive_filename:
            self.libraries = []
            
        else:
            # add the source information
            for lib in self.libraries:
                if lib[0] == 'unqlite':
                    lib[1]['sources']= [os.path.join(self._dest_folder, 'unqlite.c')]
                    lib[1]['include_dirs'] = []


    def run(self):
        # skip this step if there are no libraries to build
        if not self.libraries or len(self.libraries) <= 0:
            return

        # ensure the destination folder exists
        log.info('Checking folder: ' + self._dest_folder)
        if not os.path.exists(self._dest_folder):
            log.info('Creating folder: ' + self._dest_folder)
            if not self.dry_run:
                os.makedirs(self._dest_folder)
        
        # check if the archive exists before downloading it
        archive_downloaded = False
        unqlite_archive_path = os.path.join(self._dest_folder, self._unqlite_archive_filename)
        log.info('Checking if archive downloaded: ' + self._unqlite_archive_filename)
        if not os.path.exists(unqlite_archive_path):
            
            try:
                log.info('Downloading {0} to folder {1}'.format(
                    self._unqlite_download_url, self._dest_folder))
                if not self.dry_run:
                    download = urlopen(self._unqlite_download_url)
                    with open(unqlite_archive_path, 'wb') as local_archive_file:
                        local_archive_file.write(download.read())
                    archive_downloaded = True
                    
            except HTTPError, e:
                log.error('HTTP Error: {0} {1}'.format(e.code, self._unqlite_download_url))
                raise
            except URLError, e:
                log.error('URL Error: {0} {1}'.format(e.reason, self._unqlite_download_url))
                raise        
        
        # extract the source files only if they don't exist or no recent
        # download was made
        log.info('Checking if archive extracted: ' + self._unqlite_archive_filename)
        source_extracted = os.path.exists(os.path.join(self._dest_folder, 'unqlite.h')) and \
            os.path.exists(os.path.join(self._dest_folder, 'unqlite.c'))
        if archive_downloaded or not source_extracted:
            log.info('Extracting archive: ' + self._unqlite_archive_filename)
            if not self.dry_run:
                with ZipFile(unqlite_archive_path) as archive_file:
                    archive_file.extractall(self._dest_folder)

        # check if the library has already been built (fix for OS X)
        # TODO this should also check timestamps of source files
        log.info('Checking if unqlite was already built')
        compiler = ccompiler.new_compiler()
        if compiler.find_library_file([self.build_clib], 'unqlite'):
            return	
                    
        # run the build
        BuildCLibBase.run(self)
