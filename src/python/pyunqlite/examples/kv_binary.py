from pyunqlite import UnqliteDatabase, UnqliteException
import sys

class TestException(Exception):
    message = 'Unknown error'
    
    def __init__(self, message):
        self.message = message
        
    def __str__(self):
        return self.message

def main(argv):
    
    # create the database
    kargs = {}
    if len(argv) >= 2:
        kargs['filename'] = argv[1]
        
    with UnqliteDatabase(**kargs) as db:
        
        # store and retrieve a string
        db.kv_store('test_str', 'Hello')
        
        res = db.kv_fetch('test_str')
        if not isinstance(res, basestring):
            raise TestException('test_str is not a string')
        
        print 'test_str = ' + res
        
        
        # store and retrieve a binary stream
        barr = bytearray.fromhex("de ad be ef")
        db.kv_store(key='test_bin', value=barr, value_len=len(barr))
        res = db.kv_fetch(key='test_bin', as_binary=True)
        if not isinstance(res, bytearray):
            raise TestException('test_bin is not a bytearray')
        
        print 'res =(' + ''.join( ['%02x ' % x for x in res] ).strip() + ')'
        
        # load an array directly
        barr2 = bytearray.fromhex("ef ef ef")
        db.kv_fetch(key='test_bin', as_binary=True, value_len=3, direct_buffer=barr2)
        print 'barr2 =(' + ''.join( ['%02x ' % x for x in barr2] ).strip() + ')'
        
        # load an array using a callback function
        def test_fetch_callback(cb_data, cb_data_len):
            print '(callback)test = "{0}" with length={1}'.format(
                ''.join( ['%02x ' % x for x in cb_data] ).strip(),
                cb_data_len)
            return True
            
        db.kv_fetch(key='test_bin', as_binary=True, callback=test_fetch_callback)
        

# Main program
if __name__ == "__main__":
    try:
        main(sys.argv)

    except UnqliteException as e:
        print >>sys.stderr, 'unqlite error: ' + str(e)
        sys.exit(1)

    except Exception as e:
        print >>sys.stderr, 'runtime error: ' + str(e)
        sys.exit(1)
