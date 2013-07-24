#from mock.pyunqlite import UnqliteDatabase
from pyunqlite import UnqliteDatabase
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
        for _ in xrange(20):
            key = db.util.random(string_len=11)
            db.kv_store(key, data)
        
        # delete a record
        db.kv_delete('test')
        
        print 'Done inserts.'
        print 'Starting the iteration process...'
        
        for entry in db.kv_cursor():
            print 'db({0})={1}'.format(entry.key, entry.data)
        
        print 'Finished iteration process'
      

# Main program
if __name__ == "__main__":
    try:
        main(sys.argv)

    except Exception as e:
        print >>sys.stderr, e
        sys.exit(1)
