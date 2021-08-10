# trampoline.py
#
# A simple of example of trampoling between coroutines

# A subroutine
def add(x,y):
    yield x+y

# A function that calls a subroutine
def main():
    r = yield add(2,2)
    print r
    yield

def run():
    m      = main()       
    # An example of a "trampoline"
    sub    = m.send(None)             
    result = sub.send(None)
    m.send(result)

run()

