import os
import unittest

def suite():
    loader = unittest.TestLoader()
    tests_root_dir = os.path.dirname(os.path.abspath(__file__))
    return loader.discover(start_dir=tests_root_dir)

if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())

