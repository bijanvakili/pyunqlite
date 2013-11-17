import _pyunqliteimp
import contextlib

UnqliteException = _pyunqliteimp.UnqliteException

# thin wrapper for cursor seek constants
SEEK_MATCH_EXACT = _pyunqliteimp.SEEK_MATCH_EXACT
SEEK_MATCH_LE = _pyunqliteimp.SEEK_MATCH_LE
SEEK_MATCH_GE = _pyunqliteimp.SEEK_MATCH_GE

class UnqliteDatabase(_pyunqliteimp.UnqliteDatabaseImp):
    """
    An UnQLite database
    
    :param filename: Path to database.  Using the default ``':mem:'`` will create the database in memory
    :type filename: str
    :param create: ``True`` to open with read/write priveleges ...
    :type create: bool
    :param read_only: ``True`` to open as read only.  Default is ``False``
    :type read_only: bool
    :param temporary: A private, temporary on-disk database is created.  This private database will be automatically deleted as soon as the database connection is closed.  Default is ``False``
    :type temporary: bool
    :param use_journaling: Default ``True`` to include journaling.  Set to ``False`` to omit journaling (not recommended).
    :type use_journaling: bool
    :param use_mutex: Default ``True`` to include private mutex on database handle.  Set to ``False`` to omit mutex (not recommended).
    :type use_mutex: bool

    If you opened thedatabase by calling ``UnqliteDatabase()`` directly, then you must call ``close()``
    Otherwise, it is recommended to use a context managed inside a ``with`` statement to close the database automatically::
    
        with UnqliteDatabase(...) as db: 
            ....

    """
    
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
        Fetch a record from the database.

        :param key: Record key.
        :type key: str
        :raises: KeyError.

        """
        try:
            return self.kv_fetch(key)
        except UnqliteException:
            raise KeyError(key)

    def __setitem__(self, key, value):
        """
        Stores records in the database.

        :param key: Record key.
        :type key: str
        :param value: Data value.
        :type value: any

        """
        return self.kv_store(key, value)

    def __delitem__(self, key):
        """
        Remove a record from the database.

        :param key: Record key

        """
        return self.kv_delete(key)
    
    def __contains__(self, key):
        """
        Determines if a key exists in the table.

        :param key: Record key.

        """
        return self.kv_exists(key)

    def get_cursor_context(self):
        return contextlib.closing(self.kv_cursor())

    
    def _init_cursor(self, cursor, backwards, start, match_type):
        # determine the appropriate starting point
        if start:
            cursor.seek(key=start, match_type=match_type)
        elif backwards:
            cursor.seek_end()
        else:
            cursor.seek_begin()
        
    
    def itercursor(self, start=None, match_type=SEEK_MATCH_EXACT, backwards=False):
        """
        Iterates through the database using a cursor.
        
        :param start: Starting key.  Defaults to ``None``
        :type key: str
        :param match_type: Matching type for starting key (see Iterating_ for details).
        :param backwards: ``True`` to iterate backwards, ``False`` otherwise (default)
        :type backwards: bool
        """
        
        with self.get_cursor_context() as cursor:
            
            # determine the appropriate starting point
            self._init_cursor(cursor, backwards, start, match_type)
            
            while cursor.is_valid():
                yield cursor
                
                # move the cursor
                cursor.next() if not backwards else cursor.prev()
    
    def __iter__(self):
        """
        Iterates through all the keys in the database in forward sequential order.
        
        See Iterating_ for details.

        """
        return self.iterkeys()

    def __reversed__(self):
        """
        Iterates through all the keys in the database in reverse sequential order.
        
        See Iterating_ for details.

        """
        return self.iterkeys(backwards=True)

    def iterkeys(self, **kvargs):
        """
        Iterates through all the keys in the database in sequential order.
        
        See Iterating_ for details.

        """
        for cursor in self.itercursor(**kvargs):
            yield cursor.get_key()
        
    def itervalues(self, **kvargs):
        """
        Iterates through all the values in the database in sequential order.
        
        See Iterating_ for details.

        """
        as_binary = kvargs.get('as_binary', False)
        for cursor in self.itercursor(**kvargs):
            yield cursor.get_data(as_binary=as_binary)
    
    def iteritems(self, **kvargs):
        """
        Return an iterator over the database's (key, value) pairs.

        See Iterating_ for details.

        """
        as_binary = kvargs.get('as_binary', False)
        for cursor in self.itercursor(**kvargs):
            yield (cursor.get_key(), cursor.get_data(as_binary=as_binary), )
        
    def clear(self):
        """
        Removes all records from the database.

        """
        with self.get_cursor_context() as cursor:
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
        
        ``fromkeys()`` is a class method that returns a new dictionary. `value` defaults to None.
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
        Return a copy of the dictionary's list of (key, value) pairs.

        """
        # TODO Optimize to use callbacks
        results = []
        for (k,v) in self.iteritems():
            results.append((k,v,))
        return results
    
    def keys(self):
        """
        Return a copy of the dictionary's list of keys. 

        """
        results = []
        def collect_keys(*args): 
            results.append(args[0])
            return True
        
        for cursor in self.itercursor():
            cursor.get_key(callback=collect_keys) 

        return results
    
    def values(self, as_binary=False):
        """
        Return a copy of the dictionary's list of values. 
        
        :param as_binary: ``True`` to retrieve values as ``bytearray``, ``False`` as ``str`` (default) 
        :type key: bool
        
        """
        results = []
        def collect_values(*args): 
            results.append(args[0])
            return True 
        
        for cursor in self.itercursor():
            cursor.get_data(callback=collect_values, as_binary=as_binary) 

        return results

    
    def pop(self, key, default=None):
        """
        If key is in the dictionary, remove it and return its value, else return default.

        If default is not given and key is not in the dictionary, a KeyError is raised.
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
        with self.get_cursor_context() as cursor:
            if cursor.is_valid():
                cursor.remove()
            else:
                raise KeyError()
    
    def setdefault(self, key, default=None):
        """
        If key is in the dictionary, return its value. 

        If not, insert key with a value of default and return default. default defaults to None.

        """
        # TODO This could be optimized in C++ to reduce lookups
        if key in self:
            return self[key]
        else:
            self[key] = default
            return default
        
    def update(self, *args):
        """
        Update the dictionary with the key/value pairs from other, overwriting existing keys. 

        :return: None
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
        Compiles and returns a VM.

        :param filename: path to JX9 script
        :type filename: str
        :param jx9_content: optional string containing JX9 script
        :type jx9_content: str
        :param jx9_content_len: optional length of JX9 script string
        :type jx9_content_len: int
        :returns: An UnQLite JX9 virtual machine
        :raises: UnqliteException - An error occurred creating this virtual machine
        """
        return contextlib.closing(
            super(UnqliteDatabase, self).vm_compile(
                filename, jx9_content, jx9_content_len))
