
class UnqliteException(Exception):
    """ 
    Exceptions for unqlite
    """
    
    message = _('Unknown error')
    
    def __init__(self, error_code=None, message=None):
        if (error_code):
            self._error_code = error_code
            
        # translate the error code
            
        self._message = message
    
    def error_code(self):
        return self._error_code
    
    def __str__(self):
        return self.message
    

class UnqliteDatabase:
    # opens the database
    def __init__(self, 
                 filename=':mem:', create=True, read_only=False, temporary=False,
                 use_journaling=True, use_mutex=True):
        self.util = UnqliteUtil(self)
        return self
    
    # closes the database
    def close(self):
        pass
    
    def kv_store(self, key, value,
                 key_len=-1, value_len=None, append=False):
        if (not value_len):
            value_len=len(value)
        pass

    def kv_fetch(self, key, 
                 key_len=-1, query_len=False):
        if query_len:
            return 0
        else:
            return ''

    def kv_delete(self, key,
                  key_len=-1):
        pass
    
    def kv_cursor(self, start=None):
        # yield bla bla
        pass

class UnqliteUtil:
    def __init__(self, db):
        self._db = db
        
    def random(self, string_len=None):
        if string_len:
            return 'random'
        else:
            return 0
    