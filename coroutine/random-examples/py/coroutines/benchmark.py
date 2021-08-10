# benchmark.py
#
# A micro benchmark comparing the performance of sending messages into
# a coroutine vs. sending messages into an object

# An object
class GrepHandler(object):
    def __init__(self,pattern, target):
        self.pattern = pattern
        self.target = target
    def send(self,line):
        if self.pattern in line:
            self.target.send(line)

# A coroutine
from coroutine import coroutine

@coroutine
def grep(pattern,target):
    while True:
        line = (yield)
        if pattern in line:
            target.send(line)

# A null-sink to send data
@coroutine
def null(): 
    while True: item = (yield)

# A benchmark
line = 'python is nice'
p1   = grep('python',null())          # Coroutine
p2   = GrepHandler('python',null())   # Object

from timeit import timeit

print "coroutine:", timeit("p1.send(line)",
                          "from __main__ import line, p1")

print "object:", timeit("p2.send(line)",
                        "from __main__ import line, p2")
