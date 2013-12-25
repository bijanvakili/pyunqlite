import unittest
from pyunqlite import UnqliteDatabase, UnqliteException

class VirtualMachineTestCase(unittest.TestCase):
    
    def setUp(self):
        self.db = UnqliteDatabase()
        self.vm = None

    def tearDown(self):
        if self.vm:
            self.vm.close()
            self.vm = None
        
        if self.db:
            self.db.close()
            self.db = None

    def _check_constant(self, constant_callback):
        jx9_script = '$result = __TEST__;'
        self.vm = self.db.vm_compile(jx9_content=jx9_script, no_context=True)
        self.vm.create_constant('__TEST__', callback=constant_callback)
        self.vm.execute()
        expected_value = constant_callback(None)
        if (type(expected_value) is bytearray):
            actual_result = self.vm.get_variable('result', len(expected_value))
        else:
            actual_result = self.vm.get_variable('result')
        
        self.assertEqual(actual_result, constant_callback(None))
        self.vm.close()
       
    def test_constants(self):
        def bool_constant(user_data):
            return True
        def string_constant(user_data):
            return 'test'  
        def null_constant(user_data):
            return None
        def int_constant(user_data):
            return 5
        def float_constant(user_data):
            return 7.6
        def byte_constant(user_data):
            return bytearray.fromhex("de ad be 55")
        
        self._check_constant(bool_constant)
        self._check_constant(string_constant)
        self._check_constant(null_constant)
        self._check_constant(int_constant)
        self._check_constant(float_constant)
        self._check_constant(byte_constant)
        
    
    def test_bad_constant(self):
        
        def string_constant(user_data):
            return 'test'  

        # test adding and removing the constant
        jx9_script = '$result = __TEST__;'
        self.vm = self.db.vm_compile(jx9_content=jx9_script, no_context=True)
        self.vm.create_constant('__TEST__', callback=string_constant)
        self.vm.execute()
        
        # try removing a non-existtent constant
        with self.assertRaises(UnqliteException):
            self.vm.delete_constant('__NONEXISTENT__')
            
        # try retrieving an invalid variable
        with self.assertRaises(UnqliteException):
            self.vm.get_variable('non-existent')
        
        # remove the constant and ensure the VM fails
        self.vm.delete_constant('__TEST__')
        with self.assertRaises(UnqliteException):
            self.vm.execute()
        
        # try deleting constant after it has already been removed
        with self.assertRaises(UnqliteException):
            self.vm.delete_constant('__TEST__')
