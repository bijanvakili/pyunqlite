.. pyunqlite documentation master file, created by
   sphinx-quickstart on Mon Oct 14 20:32:07 2013.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

pyunqlite 
=========
A python client library for the UnQLite embedded NoSQL database


pyunqlite (*module*)
--------------------

.. automodule:: pyunqlite
.. autoclass:: UnqliteDatabase
   :members:
   :exclude-members: acquire, append, disown, next, own, 
   :special-members: __getitem__, __setitem__, __delitem__, __contains__, __iter__, __reversed__
   :inherited-members:

Iterating
+++++++++
Common parameters:

* `backwards` (bool): ``True`` to iterate backwards, ``False`` to iterate forwards (default)
* `values_as_binary` (bool): If ``True``, each iteration will return record values as binary
* `keys_callback` (callback): Optional callback function for record keys
* `values_callback` (callback): Optional callback function for record values
* `start` (callback): Optional starting key for iteration 
* `match_type`: Optional key matching method if 'start' is specified. 

  * ``SEEK_MATCH_EXACT`` -- match exactly
  * ``SEEK_MATCH_LE`` -- less than or equal 
  * ``SEEK_MATCH_GE`` -- less than or equal

Callback functions have the following parameters:

* `cb_data` (string or bytearray) : Data.
* `cb_data_len` (int): Length of data (excluding null terminator for strings).

Error Handling
++++++++++++++

.. autoexception:: UnqliteException
   :members:

