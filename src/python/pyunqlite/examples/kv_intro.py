from pyunqlite import UnqliteDatabase, UnqliteException
import sys

def main(argv):
    
    # create the database
    kargs = {}
    if len(argv) >= 2:
        kargs['filename'] = argv[1]
        
    with UnqliteDatabase(**kargs) as db: 
        # store some records
        db['test'] = 'Hello'
        
        # a small formatted string
        db['date'] = 'dummy date: {0}:{1}:{2}'.format(2013, 06, 07)
        
        # switch to append interface
        db.kv_store('msg', 'Hello, ', append=True)
        db.kv_store('msg', 'dummy time is: ', append=True)
        db.kv_store('msg', '{0}:{1}:{2}'.format(10, 16, 53), append=True)
        
        try:
            # store 20 random records
            data = 'dummy data'
            for i in xrange(20):
                key = db.util_random_string(len=11)
                db[key] = data
        except UnqliteException:
            # insertion failed, roll back the database
            db.rollback()
            
        # delete a record
        del db['test']
        
        print 'Done...Starting the iteration process...'
        
        # using callbacks and the cursor directly for efficiency
        def data_consumer_callback(cb_data, cb_data_len):
            print '\t{0}'.format(cb_data)
            return True

        for cursor in db.itercursor():
            key_len = cursor.get_key_len()
            print 'Key ==> {0}'.format(key_len)
            cursor.get_key(callback=data_consumer_callback)
            
            data_len = cursor.get_data_len()
            print 'Data ==> {0}'.format(data_len)
            cursor.get_data(callback=data_consumer_callback)

        """
        # a simpler method, but least efficient
        for k,v in db.iteritems():
            print 'db({0})={1}'.format(k, v)
        """


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
