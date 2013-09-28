from pyunqlite import UnqliteDatabase, UnqliteException
import os
import sys

def data_consumer(cb_data, cb_data_len):
    print cb_data
    return True

def main(argv):
    
    # parse the command line
    kargs = {}
    if len(argv) >= 2:
        kargs['filename'] = argv[1]
    
    # open the database
    with UnqliteDatabase(**kargs) as db:
        
        # create the JX9 virtual machine 
        with db.vm_compile(filename=os.path.join('jx9', 'db_intro.jx9.txt')) as vm:
            
            # run the virtual machine
            vm.execute(callback=data_consumer)

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
