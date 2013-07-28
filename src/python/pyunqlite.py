import pyunqliteimp
import _pyunqliteimp

# thin wrapper for exception
UnqliteException = _pyunqliteimp.UnqliteException


class UnqliteDatabase(pyunqliteimp.UnqliteDatabaseImp):
    
    """
    context managers ('with' statement)
    """
    # TODO Switch to using SWIG %extend?
    def __enter__(self):
        return self
    
    # TODO Switch to using SWIG %extend?
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

    """
    iteration
    """
    def kv_cursor(self, start=None):
        # TODO Implement 'start'
        cursor = super(UnqliteDatabase, self).kv_cursor()
        while cursor.is_valid():
            yield UnqliteEntry(cursor.get_key(), cursor.get_data())
            cursor.next()
        
class UnqliteEntry:
    def __init__(self, key, data):
        self.key = key
        self.data = data
