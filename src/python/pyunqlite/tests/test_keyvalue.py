import unittest
from pyunqlite import UnqliteDatabase, UnqliteException

class KeyValueTestCase(unittest.TestCase):


    def setUp(self):
        self.db = UnqliteDatabase()


    def tearDown(self):
        self.db.close()
        self.db = None


    def test_simple_add(self):
        self.db.kv_store('test', 'hello')
        val = self.db.kv_fetch('test')
        self.assertEqual(val, 'hello')
        
        data_len = self.db.kv_fetch_len('test')
        self.assertEqual(data_len, 5)
        
        with self.assertRaises(UnqliteException):
            self.db.kv_fetch('non_existent')
        with self.assertRaises(UnqliteException):
            self.db.kv_fetch_len('non_existent')
        

    def test_simple_append(self):
        self.db.kv_store('test', 'Hello,')
        self.db.kv_store('test', ' world!', append=True)
        val = self.db.kv_fetch('test')
        self.assertEqual(val, 'Hello, world!')
        
        data_len = self.db.kv_fetch_len('test')
        self.assertEqual(data_len, 13)
        

    def test_simple_delete(self):
        self.db['test'] ='hello'
        self.assertTrue(self.db.kv_exists('test'))
        self.assertTrue('test' in self.db)
        
        self.db.kv_delete('test')
        self.assertFalse(self.db.kv_exists('test'))
        self.assertFalse('test' in self.db)


    def test_simple_callback(self):
        self.db['test'] = 'hello'
        results = {}
        
        def test_fetch_callback(cb_data, cb_data_len):
            results['cb_data'] = cb_data
            results['cb_data_len'] = cb_data_len
            return True
            
        self.db.kv_fetch('test', callback=test_fetch_callback)
        self.assertEqual(results.get('cb_data'), 'hello')
        self.assertEqual(results.get('cb_data_len'), 5)
