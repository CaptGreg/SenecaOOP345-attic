# taskcrash.py
#
# An example that shows how the initial scheduler doesn't handle
# task termination correctly.

from pyos2 import Scheduler

def foo():
    for i in xrange(10):
        print "I'm foo"
        yield
def bar():
     while True:
         print "I'm bar"
         yield

sched = Scheduler()
sched.new(foo())
sched.new(bar())
sched.mainloop()
