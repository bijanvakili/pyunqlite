import unittest
from pyunqlite import UnqliteDatabase

class UnqliteIterators(unittest.TestCase):
    def setUp(self):
        self.data = {
            '01': 'One',
            '02': 'Two',
            '03': 'Three',
            '04': 'Four',
            '05': 'Five'
        }
        self.db = UnqliteDatabase()
        for k,v in self.data.iteritems():
            self.db.kv_store(k, v)

    def tearDown(self):
        self.db.close()
        self.db = None
        
    def test_basic_operators(self):
        # TODO implement length
        # self.assertEqual( len(self.db), 5 )
        
        self.assertEqual(self.db['01'], self.data['01'])
        self.assertEqual(self.db['03'], self.data['03'])
        self.assertEqual(self.db['05'], self.data['05'])
        
        with self.assertRaises(KeyError):
            self.db['00']
        
        self.db['05'] = 'NewFive'
        self.db['06'] = 'NewSix'
        self.assertEqual(self.db['05'], 'NewFive')
        self.assertEqual(self.db['06'], 'NewSix')
        # self.assertEqual( len(self.db), 6 )
        
        del self.db['05']
        self.assertFalse('05' in self.db)
        self.assertTrue('05' not in self.db)
        self.assertTrue('04' in self.db)
        self.assertFalse('04' not in self.db)
        # self.assertEqual( len(self.db), 5 )
        
    def test_basic_methods(self):
        
        self.assertEqual(self.db.get('01', 'default'), self.data['01'])
        self.assertEqual(self.db.get('06', 'default'), 'default')
        
        self.assertTrue(self.db.has_key('01'))
        self.assertFalse(self.db.has_key('06'))
        
    def test_key_methods(self):
        
        data_keys = self.data.keys()
        for k in self.db:
            self.assertIn(k, data_keys)
        for k in self.db.iterkeys():
            self.assertIn(k, data_keys)
        
        db_keys = self.db.keys()
        self.assertEqual( len(db_keys), len(data_keys) )
        for k in db_keys:
            self.assertIn(k, data_keys)
            
        def keys_callback(cb_data, cb_data_len):
            self.assertIn(cb_data, data_keys)
            self.assertEqual(cb_data_len, len(cb_data))
            
        self.db.iterate_with_callbacks(keys_callback=keys_callback)

    def test_value_methods(self):
        
        data_values = self.data.values()
        for v in self.db.itervalues():
            self.assertIn(v, data_values)
        
        db_values = self.db.values()
        self.assertEqual( len(db_values), len(data_values) )
        for v in db_values:
            self.assertIn(v, data_values)
            
        def values_callback(cb_data, cb_data_len):
            self.assertIn(cb_data, data_values)
            self.assertEqual(cb_data_len, len(cb_data))
            
        self.db.iterate_with_callbacks(values_callback=values_callback)

        for k,v in self.db.iteritems():
            self.assertIn(k, self.data)
            self.assertEquals(v, self.data[k])

    def test_reversed_methods(self):
        results = []
        for k in reversed(self.db):
            results.append(k)
        
        reversed_keys = self.data.keys()[:]
        reversed_keys.reverse()
        
        self.assertEqual(results, reversed_keys)
        
        results = []
        def values_callback(*args):
            results.append(args[0])
        self.db.iterate_with_callbacks(backwards=True, values_callback=values_callback)
        reversed_values = self.data.values()[:]
        reversed_values.reverse()
        self.assertEqual(results, reversed_values)
        
        for k,v in self.db.iteritems(backwards=True):
            self.assertIn(k, self.data)
            self.assertEquals(v, self.data[k])
        
    def test_clear_methods(self):
        
        self.db.clear()
        for k in self.data:
            self.assertNotIn(k, self.db)
        # self.assertEqual( len(self.db), 0 )

    def test_copy_methods(self):
        
        new_db = self.db.copy()
        for k,v in new_db.iteritems():
            self.assertIn(k, self.db)
            self.assertEquals(v, self.db[k])
            
        new_db = UnqliteDatabase.fromkeys(['01', '03', '05'], value='test')
        for v in new_db.itervalues():
            self.assertEquals(v, 'test')
        new_db.close()
    
    def test_default_returns(self):
        
        self.assertEquals(self.db.get('01', 'never'), self.data['01']) 
        self.assertEquals(self.db.get('06', 'always'), 'always')
        
        self.assertEquals(self.db.setdefault('01', 'never'), self.data['01'])
        self.assertEquals(self.db.setdefault('06', 'Six'), 'Six')
        self.assertEquals(self.db['06'], 'Six')

    def test_update_methods(self):
        self.db.update({'02': 'NewTwo', '04': 'NewFour', '06': 'NewSix'})
        self.assertEquals(self.db['01'], self.data['01'])
        self.assertEquals(self.db['02'], 'NewTwo')
        self.assertEquals(self.db['03'], self.data['03'])
        self.assertEquals(self.db['04'], 'NewFour')
        self.assertEquals(self.db['05'], self.data['05'])
        self.assertEquals(self.db['06'], 'NewSix')
            
if __name__ == '__main__':
    unittest.main()
    