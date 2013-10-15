import _pyunqliteimp
import contextlib

# thin wrapper for exception
UnqliteException = _pyunqliteimp.UnqliteException

# thin wrapper for cursor seek constants
SEEK_MATCH_EXACT = _pyunqliteimp.SEEK_MATCH_EXACT
SEEK_MATCH_LE = _pyunqliteimp.SEEK_MATCH_LE
SEEK_MATCH_GE = _pyunqliteimp.SEEK_MATCH_GE

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
        Raises:
            KeyError
        """
        try:
            return self.kv_fetch(key)
        except UnqliteException:
            raise KeyError(key)
    
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
    
    def __contains__(self, key):
        """
        Determines if a key exists in the table
        Parameters:
            key: Record key
        """
        return self.kv_exists(key)

    def _get_cursor_context(self):
        return contextlib.closing(self.kv_cursor())


    """
    =========
    Iterating
    =========
    
    Common parameters include:
            backwards: True to iterate backwards, False to iterate forwards (default)
            values_as_binary: If 'true', will return record values as binary
            keys_callback: Optional callback function for record keys
            values_callback: Optional callback function for record values
            start: Optional starting key for iteration 
            match_type: Optional key matching method if 'start' is specified. 
            
    Callback functions are more efficient.  If one of 'keys_callback' or 'values_callback'
    is specified, then the iterator will not yield values and will instead use the callback
    function.
    
    Callback functions have the following parameters:
        cb_data: Data (key or value)
        cb_data_len: Length of data (excluding null terminator for strings)
            
    'match_type' for the start key can be one of:
        * SEEK_MATCH_EXACT: match exactly 
        * SEEK_MATCH_LE: less than or equal 
        * SEEK_MATCH_GE: less than or equal
    """
    
    def _init_cursor(self, cursor, backwards, start, match_type):
        # determine the appropriate starting point
        if start:
            cursor.seek(key=start, match_type=match_type)
        elif backwards:
            cursor.seek_end()
        else:
            cursor.seek_begin()
        
    
    def _iterate(self, 
                 include_keys=True, 
                 include_values=True, 
                 values_as_binary=False,
                 backwards=False,
                 start=None, 
                 match_type=SEEK_MATCH_EXACT):

        with self._get_cursor_context() as cursor:
        
            # determine the appropriate starting point
            self._init_cursor(cursor, backwards, start, match_type)
                
            while cursor.is_valid():
            
                # return data depending on the desired inclusions
                if include_keys and include_values:
                    yield (cursor.get_key(), cursor.get_data(as_binary=values_as_binary),) 
                elif include_keys:
                    yield cursor.get_key()
                elif include_values:
                    yield cursor.get_data(as_binary=values_as_binary)
                
                # move the cursor
                cursor.next() if not backwards else cursor.prev()

            
    def iterate_with_callbacks(self, 
                               keys_callback=None,
                               values_callback=None,
                               values_as_binary=False,
                               backwards=False,
                               start=None, 
                               match_type=SEEK_MATCH_EXACT):

        with self._get_cursor_context() as cursor:
        
            # determine the appropriate starting point
            self._init_cursor(cursor, backwards, start, match_type)
                
            while cursor.is_valid():
            
                if keys_callback:
                    cursor.get_key(callback=keys_callback)
                if values_callback:
                    cursor.get_data(callback=values_callback, as_binary=values_as_binary)
                
                # move the cursor
                cursor.next() if not backwards else cursor.prev()

    def __iter__(self):
        """
        Iterates through all the keys in the database in forward sequential order
        
        See 'Iterating' for details.
        """
        return self.iterkeys()

    def __reversed__(self):
        """
        Iterates through all the keys in the database in reverse sequential order
        
        See 'Iterating' for details.
        """
        return self.iterkeys(backwards=True)

    def iterkeys(self, **kvargs):
        """
        Iterates through all the keys in the database in sequential order
        
        See 'Iterating' for details.
        """
        return self._iterate(include_keys=True, include_values=False, **kvargs)
        
    def itervalues(self, **kvargs):
        """
        Iterates through all the values in the database in sequential order
        
        See 'Iterating' for details.
        """
        return self._iterate(include_keys=False, include_values=True, **kvargs)
    
    def iteritems(self, **kvargs):
        """
        Return an iterator over the database's (key, value) pairs.

        See 'Iterating' for details.
        """
        return self._iterate(include_keys=True, include_values=True, **kvargs)
        
    # TODO implement similar methods from python dict()
    def clear(self):
        """
        Removes all records from the database
        """
        with self._get_cursor_context() as cursor:
            while cursor.is_valid():
                cursor.remove()
            
    def copy(self, **kvargs):
        """
        Return a shallow copy of the dictionary.
        """
        # TODO convert to use callbacks
        new_db = UnqliteDatabase(**kvargs)
        for k,v in self.iteritems():
            new_db[k] = v
            
        return new_db
            
    @classmethod
    def fromkeys(cls, seq, value=None, **kvargs):
        """
        Create a new dictionary with keys from seq and values set to value.
        
        fromkeys() is a class method that returns a new dictionary. value defaults to None.
        """
        new_db = UnqliteDatabase(**kvargs)
        for k in seq:
            new_db[k] = value
           
        return new_db
    
    def get(self, key, default=None):
        """
        Return the value for key if key is in the dictionary, else default. 
        If default is not given, it defaults to None, so that this method never raises a KeyError.
        """
        # TODO This could be optimized in C++ to do only one lookup
        if key in self:
            return self[key]
        else:
            return default
        
    def has_key(self, key):
        """
        Test for the presence of key in the dictionary. has_key() is deprecated in favor of key in d.
        """
        return key in self
    
    def items(self):
        """
        Return a copy of the dictionary’s list of (key, value) pairs.
        """
        # TODO Optimize to use callbacks
        results = []
        for (k,v) in self.iteritems():
            results.append((k,v,))
        return results
    
    def keys(self):
        """
        Return a copy of the dictionary’s list of keys. 
        """
        results = []
        def callback(*args): 
            results.append(args[0])
            return True 
        self.iterate_with_callbacks(keys_callback=callback)
        return results
    
    def values(self):
        """
        Return a copy of the dictionary’s list of values. 
        """
        results = []
        def callback(*args): 
            results.append(args[0])
            return True 
        self.iterate_with_callbacks(values_callback=callback)
        return results

    
    def pop(self, key, default=None):
        """
        If key is in the dictionary, remove it and return its value, else return default. If default is not given and key is not in the dictionary, a KeyError is raised.
        """
        # TODO This could be optimized in C++ to do only one lookup
        if key in self:
            value = self[key]
            del self[key]
            return value
        elif default:
            return default
        else:
            raise KeyError(key)
        
    def popitem(self):
        """
        Remove and return an arbitrary (key, value) pair from the dictionary.
        """
        with self._get_cursor_context() as cursor:
            if cursor.is_valid():
                cursor.remove()
            else:
                raise KeyError()
    
    def setdefault(self, key, default=None):
        """
        If key is in the dictionary, return its value. If not, insert key with a value of default and return default. default defaults to None.
        """
        # TODO This could be optimized in C++ to reduce lookups
        if key in self:
            return self[key]
        else:
            self[key] = default
            return default
        
    def update(self, *args):
        """
        Update the dictionary with the key/value pairs from other, overwriting existing keys. Return None.
        """
        if len(args) > 1:
            d = dict(args)
        else:
            d = args[0]
        for k,v in d.iteritems():
            self[k] = v
        return None

    # TODO Implement database view routines
    def viewitems(self):
        raise NotImplementedError('Dictionary view not implemented yet')
        
    def viewkeys(self):
        raise NotImplementedError('Dictionary view not implemented yet')
    
    def viewvalues(self):
        raise NotImplementedError('Dictionary view not implemented yet')
           
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
