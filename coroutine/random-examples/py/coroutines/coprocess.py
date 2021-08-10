# coprocess.py
#
# An example of running a coroutine in a subprocess connected by a pipe

import cPickle as pickle
from coroutine import *

@coroutine
def sendto(f):
    try:
        while True:
            item = (yield)
            pickle.dump(item,f)
            f.flush()
    except StopIteration:
        f.close()

def recvfrom(f,target):
    try:
        while True:
            item = pickle.load(f)
            target.send(item)
    except EOFError:
        target.close()


# Example use
if __name__ == '__main__':
    import xml.sax
    from cosax import EventHandler
    from buses import *

    import subprocess
    p = subprocess.Popen(['python','busproc.py'],
                         stdin=subprocess.PIPE)

    xml.sax.parse("allroutes.xml",
                  EventHandler(
                          buses_to_dicts(
                          sendto(p.stdin))))
