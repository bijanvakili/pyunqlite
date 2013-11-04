from pyunqlite import UnqliteDatabase, UnqliteException
import sys

NUM_BAD_RECORDS=10
MAX_RECORDS = 100000

def data_consumer_callback(cb_data, cb_data_len):
    try:
        print cb_data
    except:
        return False
    
    return True


def main(argv):
    
    # create the database
    kargs = {}
    if len(argv) >= 2:
        kargs['filename'] = argv[1]
        
    with UnqliteDatabase(**kargs) as db:
        
        print 'The key value storage engine is: ' + db.get_kv_engine()
        db.set_db_options(auto_commit=False)
        
        # implement set_kv_options too
        
        print "Starting insertions of {0} bad records...".format(NUM_BAD_RECORDS)
        db.start_transaction()
        for i in xrange(NUM_BAD_RECORDS):

            key = 'badrecord_{0}'.format(i)
            data = db.util_random_string(32)
            db.kv_store(key=key, value=data)
        
        # force a rollback
        db.rollback()
        
        # ensure none of the records exist
        for i in xrange(NUM_BAD_RECORDS):
            key = 'badrecord_{0}'.format(i)
            value = None
            try:
                value = db.kv_fetch(key=key)
                if value:
                    raise "Oh no!  {0} was found after rollback".format(key)
            except:
                pass
         
        print "Starting insertions of {0} random records...".format(MAX_RECORDS)
        for i in xrange(MAX_RECORDS):

            key = db.util_random_string(14)
            data = db.util_random_string(32)
            db.kv_store(key=key, value=data)
            
            if i == 79125:
                db.kv_store(
                    key="sentinel", 
                    value="I'm a sentinel record insert on {0:d}{1:d}:{2:d}\n".format(14, 15, 19))
            
        if (not db.commit()):
            db.rollback()
            
        # fetch the sentinel
        db.kv_fetch('sentinel', callback=data_consumer_callback)
       

if __name__ == "__main__":
    try:
        main(sys.argv)

    except UnqliteException as e:
        print >>sys.stderr, 'unqlite error: ' + str(e)
        sys.exit(1)

    except Exception as e:
        print >>sys.stderr, 'runtime error: ' + str(e)
        sys.exit(1) 