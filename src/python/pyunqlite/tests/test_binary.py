import unittest
from pyunqlite import UnqliteDatabase

class BinaryDataTestCase(unittest.TestCase):
    
    def setUp(self):
        self.db = UnqliteDatabase()
        
    def tearDown(self):
        self.db.close()
        self.db = None
        
    
    def test_simple_add(self):
        
        # store and retrieve a binary stream
        barr = bytearray.fromhex("de ad be ef")
        self.db.kv_store(key='test_bin', value=barr, value_len=len(barr))
        
        stored_data = self.db.kv_fetch(key='test_bin', as_binary=True)
        
        self.assertIsInstance(stored_data, bytearray)
        self.assertEqual(stored_data, barr)
        
    def test_direct_load(self):        

        barr = bytearray.fromhex("de ad be ef")
        self.db.kv_store(key='test_bin', value=barr, value_len=len(barr))

        test_buffer = bytearray.fromhex("ef ef ef")
        self.db.kv_fetch(key='test_bin', as_binary=True, value_len=3, direct_buffer=test_buffer)
        
        self.assertEqual(len(test_buffer), 3)
        self.assertEqual(test_buffer, barr[0:3])
        
    def test_callback(self):
        
        barr = bytearray.fromhex("de ad be ef")
        self.db.kv_store(key='test_bin', value=barr, value_len=len(barr))
        
        results = {}
        def test_fetch_callback(cb_data, cb_data_len):
            results['cb_data'] = cb_data
            results['cb_data_len'] = cb_data_len
            return True
            
        self.db.kv_fetch(key='test_bin', as_binary=True, callback=test_fetch_callback)
        self.assertIsInstance(results.get('cb_data'), bytearray)
        self.assertEqual(results.get('cb_data_len'), 4)
        self.assertEqual(results.get('cb_data'), barr)
