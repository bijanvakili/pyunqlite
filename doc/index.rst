.. pyunqlite documentation master file, created by
   sphinx-quickstart on Mon Oct 14 20:32:07 2013.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

pyunqlite 
=========
A python client library for the UnQLite embedded NoSQL database

Container methods
+++++++++++++++++
For most basic operations, you can use your ``UnqliteDatabase`` like a python ``dict``
::

	# set a value
	db['key'] = value
	
	# retrieve a value
	value = db['key']
	value = db.get('key', default=None)
	
	# delete a value
	del db['key']
	
	# checking for a value
	if 'key' in db:
		print "'key' was found."
	else:
		print "'key' was not found."

Using Callbacks
+++++++++++++++
Data stored in the database can be very large.  This could lead to inefficient retrieval due to copy by value.

Therefore, it is recommend you use callbacks to ensure copy-by-reference semantics:
::

   db['key'] = value

   MAX_VALUE_LEN = ...
   v = None

   # data consumer callback
   def data_callback(cb_data, cb_data_len):
      if cb_data_len <= MAX_VALUE_LEN:
         v = cb_data
      else:
         v = None
      return True

   db.kv_fetch('test', callback=data_callback)
   if v:
      ...

Iterating
+++++++++
You can also iterate over the database like a python ``dict``:
::

	# iterate with key and values
	for k,v in db.iteritems():
	    print 'db({0})={1}'.format(k, v)

	# iterate with keys only
	for k in db.iterkeys():
	    print 'db({0})'.format(k)
	for k in db:
	    print 'db({0})'.format(k)

	# iterate with values only
	for v in db.itervalues():
	    print 'value = {0}'.format(v)

However, individual data items can be very large.  Therefore, the most efficient method of iterating with very large data values is to use *callbacks*:
::

   # iterate values using a callback
   v = None

   # data consumer callback for cursor
   def test_cursor_data_callback(cb_data, cb_data_len):
      v = cb_data
      return True

   # iterate using the cursor
   for c in db.itercursor():
      k = c.get_key()
      c.get_value(callback=test_cursor_data_callback)
      print 'db({0})={1}'.format(k, v)

You can also perform randomized cursor control by directly using the cursor rather than an iterator:
::

   # use a cursor inside a 'with' context
   with cursor in db.get_cursor_context():

      ...
      # position the cursor based on a search key
      cursor.seek(key='...', match_type=SEEK_MATCH_EXACT)
      ...

      # keep looping while the cursor is in a valid position
      while cursor.is_valid():
         ...
         # move the cursor forwards or backwards
         if (...):
            cursor.next()
         else:
            cursor.prev()
		
Iterator (function prefixed with ``iter``) have the following parameters:

* `start` (callback): Optional starting key for iteration 
* `match_type`: Optional key matching method if 'start' is specified. 
* `backwards` (bool): ``True`` to iterate backwards, ``False`` to iterate forwards (default)
* `as_binary` (bool): If ``True``, each iteration will return record values as binary

  * ``SEEK_MATCH_EXACT`` -- match exactly
  * ``SEEK_MATCH_LE`` -- less than or equal 
  * ``SEEK_MATCH_GE`` -- less than or equal

Callback functions have the following parameters:

* `cb_data` (string or bytearray) : Data.
* `cb_data_len` (int): Length of data (excluding null terminator for strings).

Binary Data
+++++++++++
You can store and retrieve non-string binary data using the python ``bytearray`` type.
::

   # store a binary stream
   barr = bytearray.fromhex("a0 b1 ...")
   db.kv_store(key='test_binary', value=barr, value_len=len(barr))
   
   # retrieve the binary stream as a function result
   binary_data = db.kv_fetch(key='test_binary', as_binary=True)
   
   # retrieve the binary stream using a callback
   def test_cursor_data_callback(cb_data, cb_data_len):
      ...
      
   db.kv_fetch(key='test_binary', as_binary=True, callback=test_fetch_callback)

   # retrieve binary data from an iterator
   for c in db.itercursor():
      ...
      binary_data = c.get_value(as_binary=True)
      
      ...
      c.get_value(as_binary=True, callback=test_cursor_data_callback)
      
      
You can also load data directly into a pre-existinb ``bytearray`` buffer.
::

   buffer = bytearray.fromhex("00 00 00")
   db.kv_fetch(key='test_binary', as_binary=True, value_len=3, direct_buffer=buffer)

   ...

   for c in db.itercursor():
      ...
      c.get_value(as_binary=True, value_len=3, direct_buffer=buffer)


Virtual Machines
++++++++++++++++
You can create virtual machines to operate on the UnQLite database
::

   # a user defined constant
   def PI_constant(user_data):
      return 3.1415926535898

   # open a virtual machine within a context
   with db.vm_compile(filename=os.path.join(examples_dir, 'jx9', 'db_intro.jx9.txt')) as vm:

      # set up constants
      vm.create_constant('__PI__', callback=PI_constant)

      # run the virtual machine
      vm.execute(callback=data_consumer)

      # output the result
      print 'Result = {0}'.format(self.vm.get_variable('result'))


Module Reference
++++++++++++++++

.. automodule:: pyunqlite
.. autoclass:: UnqliteDatabase
   :members:
   :inherited-members:
   :exclude-members: acquire, append, disown, next, own, __cmp__, __delattr__, __format__, __ge__, __getattribute__, __gt__, __hash__, __hex__, __init__, __le__, __long__, __lt__, __ne__, __new__, __oct__, __reduce__, __reduce_ex__, __repr__, __setattr__, __sizeof__, __str__, __subclasshook__, __wearef__ 
   :special-members: __getitem__, __setitem__, __delitem__, __contains__, __iter__, __reversed__ 

.. autoclass:: UnqliteCursor
   :members:
   :inherited-members:
   :exclude-members: acquire, append, disown, next, own
 
.. autoclass:: UnqliteVirtualMachine
   :members:
   :inherited-members:
   :exclude-members: acquire, append, disown, next, own
 
.. autoexception:: UnqliteException
   :members:

