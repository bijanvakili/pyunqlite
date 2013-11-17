import unittest
from test_iterators import IteratorsTestCase
from test_keyvalue import KeyValueTestCase

def suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.TestLoader().loadTestsFromTestCase(IteratorsTestCase))
    suite.addTest(unittest.TestLoader().loadTestsFromTestCase(KeyValueTestCase))
    return suite

if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite())
