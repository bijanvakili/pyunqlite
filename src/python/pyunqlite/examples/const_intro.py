from pyunqlite import UnqliteDatabase, UnqliteException
import os
import sys
import time

def PI_constant(user_data):
    return 3.1415926535898

def TIME_constant(user_data):
    time_str = time.strftime('%H:%M:%S', time.localtime())
    return time_str

def OS_constant(user_data):
    return 'Windows' if os.name == 'nt' else 'UNIX'  

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
        examples_dir = os.path.dirname(os.path.abspath(__file__))
        with db.vm_compile(filename=os.path.join(examples_dir, 'jx9', 'const_intro.jx9.txt')) as vm:
            
            # setup the constants
            vm.create_constant('__PI__', callback=PI_constant)
            
            user_data = 'my data'
            vm.create_constant('__TIME__', callback=TIME_constant, user_data=user_data)
            vm.create_constant('__OS__', callback=OS_constant, user_data=user_data)
            
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
