import _pyunqliteimp
import contextlib

# thin wrapper for exception
UnqliteException = _pyunqliteimp.UnqliteException


class UnqliteDatabase(_pyunqliteimp.UnqliteDatabaseImp):
    
    """
    context managers ('with' statement)
    """
    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

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
        return contextlib.closing(
            super(UnqliteDatabase, self).vm_compile(
                filename, jx9_content, jx9_content_len))
        

class UnqliteEntry:
    def __init__(self, key, data):
        self.key = key
        self.data = data
