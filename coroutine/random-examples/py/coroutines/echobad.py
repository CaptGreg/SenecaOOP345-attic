# echobad.py
#
# A flawed implementation of an concurrent echo server using our task scheduler.
# If you run this, you will notice that it freezes up right away.  This
# is because the accept() operation in the server is blocking.  Since we're
# not using threads, this blocks the operation of everything!

from pyos6 import *
from socket import *

def handle_client(client,addr):
    print "Connection from", addr
    while True:
        data = client.recv(65536)
        if not data:
            break
        client.send(data)
    client.close()
    print "Client closed"
    yield           # Make the function a generator/coroutine

def server(port):
    print "Server starting"
    sock = socket(AF_INET,SOCK_STREAM)
    sock.bind(("",port))
    sock.listen(5)
    while True:
        client,addr = sock.accept()
        yield NewTask(handle_client(client,addr))

def alive():
        while True:
            print "I'm alive!"
            yield

sched = Scheduler()
sched.new(alive())
sched.new(server(45000))
sched.mainloop()

