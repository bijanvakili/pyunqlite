import _pyunqliteimp
import contextlib

# thin wrapper for exception
UnqliteException = _pyunqliteimp.UnqliteException


class UnqliteDatabase(_pyunqliteimp.UnqliteDatabaseImp):
    
    # context managers ('with' statement)
    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()
        
    # container methods
    def __len__(self):
        # TODO create a JX9 VM and call db_total_records()
        raise NotImplementedError('Length not implemented yet')
    
    def __getitem__(self, key):
        """
        Fetch a record from the database
        Parameters:
            key: Record key
        """
        return self.kv_fetch(key)
    
    def __setitem__(self, key, value):
        """
        Stores records in the database
        Parameters:
            key: Record key
            value: Data value
        """
        return self.kv_store(key, value)

    def __delitem__(self, key):
        """
        Remove a record from the database
        Parameters:
            key: Record key
        """
        return self.kv_delete(key)
    
    def __iter__(self):
        # TODO implement using cursor callback (default arguments) and only return the key
        raise NotImplementedError('Forward iterator not implemented yet')

    def __reversed__(self):
        # TODO implement similar to above but with 'last_entry' and 'prev_entry'
        raise NotImplementedError('Reverse iterator not implemented yet')
        
    
    def __contains__(self, key):
        return self.kv_exists(key)
    
    """
    iteration
    """
    def kv_cursor(self, start=None, match_type=_pyunqliteimp.SEEK_MATCH_EXACT):
        cursor = super(UnqliteDatabase, self).kv_cursor()
        
        if start:
            cursor.seek(key=start, match_type=match_type)
        
        while cursor.is_valid():
            yield UnqliteEntry(cursor.get_key(), cursor.get_data())
            cursor.next()

    def kv_iterate_with_callbacks(self, user_callbacks, start=None, match_type=_pyunqliteimp.SEEK_MATCH_EXACT):
        cursor = super(UnqliteDatabase, self).kv_cursor()
        
        if start:
            cursor.seek(key=start, match_type=match_type)
        
        while cursor.is_valid():
            cursor.get_key(callback=user_callbacks[0])
            cursor.get_data(callback=user_callbacks[1])
            cursor.next()
            
    def vm_compile(self, filename=None, jx9_content=None, jx9_content_len=-1):
        """
        Compiles and returns a VM
        Parameters:
            filename: path to JX9 script
            jx9_content: optional string containing JX9 script
            jx9_content_len: optional length of JX9 script string
        Returns:
            An UnQLite JX9 virtual machine
        Raises:
            UnqliteException: An error occurred creating this virtual machine
        """
        return contextlib.closing(
            super(UnqliteDatabase, self).vm_compile(
                filename, jx9_content, jx9_content_len))
        

class UnqliteEntry:
    def __init__(self, key, data):
        self.key = key
        self.data = data
