# cocrash.py
#
# An example of hooking coroutines up in a way that might cause a potential
# crash.   Basically, there are two threads feeding data into the
# printer() coroutine.    

from cobroadcast import *
from cothread import threaded

p = printer()
target = broadcast([threaded(grep('foo',p)),
                    threaded(grep('bar',p))])

# Adjust the count if this doesn't cause a crash
for i in xrange(10):
    target.send("foo is nice\n")
    target.send("bar is bad\n")

del target
del p

