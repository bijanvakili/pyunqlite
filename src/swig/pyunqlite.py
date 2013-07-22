from pyunqliteimp import UnqliteDatabaseImp

class UnqliteDatabase:
    # opens the database
    def __init__(self, 
                 filename=':mem:', create=True, read_only=False, temporary=False,
                 use_journaling=True, use_mutex=True):
        
        # setup the database and utility handle
        self._db = UnqliteDatabaseImp(filename, create, read_only,
                                      temporary, use_journaling, use_mutex)
        
        self.util = UnqliteUtil(self._db)
    
    def __del__(self):
        # cleanup
        self.close()
    
    # closes the database
    def close(self):
        # explicit removal of references to force closure
        if (getattr(self ,'util', None)):
            del self.util
        if (getattr(self,'_db', None)):
            del self._db


    """
    context managers ('with' statement)
    """
    
    def __enter__(self):
        return self
    
    
    def __exit__(self, **kvargs):
        self.close()

    """
    Method overrides
    """
    def kv_store(self, key, value,
                 key_len=-1, value_len=None, append=False):
        
        # TODO 'ken_len' and 'value_len' are ignored
        if (not value_len):
            value_len=len(value)
            
        self._db.kv_store(key, value, append)

    def kv_fetch(self, key, 
                 key_len=-1, query_len=False):
        
        # TODO Implement 'key_len' and 'query_len'
        value = ''
        self._db.kv_fetch(key, value)
        
        return value

    def kv_delete(self, key,
                  key_len=-1):
        # TODO Implement 'key_len'
        self._db.kv_delete(key)
    
    def kv_cursor(self, start=None):
        # TODO Implement 'start'
        cursor = self._db.kv_cursor()
        while cursor.is_valid():
            yield UnqliteEntry(cursor.get_key(), cursor.get_data())
            cursor.next()
        
class UnqliteEntry:
    def __init__(self, key, data):
        self.key = key
        self.data = data

class UnqliteUtil:
    def __init__(self, db):
        self._db = db
        
    def random(self, string_len=None):
        if string_len:
            return self._db.util_random_string(string_len)
        else:
            return self._db.util_random_int()
    