
import os.path

_EXCLUDED_FILES = ['__init__.py']

def collect_example_filelist(examples_dir):
    """
    Returns relative pathnames to all example files
    """
    all_example_paths = []
    for f in os.listdir(examples_dir):
        rel_path = os.path.join(examples_dir, f)
        ext = os.path.splitext(rel_path)[1]
        if os.path.isfile(rel_path) and \
            ext != '.pyc' and \
            not f in _EXCLUDED_FILES:
            
            all_example_paths.append(rel_path)
              
    return all_example_paths 
