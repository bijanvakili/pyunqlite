from pyunqlite import UnqliteDatabase, UnqliteException
import sys

def main(argv):
    
    # create the database
    kargs = {}
    if len(argv) >= 2:
        kargs['filename'] = argv[1]
        
    with UnqliteDatabase(**kargs) as db: 
        # store some records
        db.kv_store('test', 'Hello')
        db.kv_store('date', 'dummy date: {0}:{1}:{2}'.format(2013, 06, 07))
        
        # append some values
        db.kv_store('msg', 'Hello, ', append=True)
        db.kv_store('msg', 'dummy time is: ', append=True)
        db.kv_store('msg', '{0}:{1}:{2}'.format(10, 16, 53), append=True)
        
        # store 20 random records
        data = 'dummy data'
        halfway_key = None
        for i in xrange(20):
            key = db.util_random_string(len=11)
            if i == 10:
                halfway_key = key
            db.kv_store(key, data)

        # retrieve a record
        data = db.kv_fetch('test')
        data_len = db.kv_fetch_len('test')
        print 'test = "{0}" with length={1}'.format(data, data_len)
        
        # retrieve a record using a callback
        def test_fetch_callback(cb_data, cb_data_len):
            print '(callback)test = "{0}" with length={1}'.format(
                cb_data, cb_data_len)
            return True
            
        db.kv_fetch('test', callback=test_fetch_callback)
        
        # delete a record
        db.kv_delete('test')
        
        print 'Done inserts.'
        print 'Starting the iteration process...'
        
        for entry in db.kv_cursor():
            print 'db({0})={1}'.format(entry.key, entry.data)
            
        print 'Restart iteration at selected key: ' + halfway_key
        for entry in db.kv_cursor(start=halfway_key):
            print 'db({0})={1}'.format(entry.key, entry.data)
  
        
        def test_cursor_key_callback(cb_data, cb_data_len):
            print '(callback)key={0}'.format(cb_data)
            return True
        
        def test_cursor_data_callback(cb_data, cb_data_len):
            print '(callback)value={0}'.format(cb_data)
            return True
        
        print 'Restart iteration and use callback functions...'
        db.kv_iterate_with_callbacks(user_callbacks=(test_cursor_key_callback, test_cursor_data_callback,))
        print 'Finished iteration process'
      

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
