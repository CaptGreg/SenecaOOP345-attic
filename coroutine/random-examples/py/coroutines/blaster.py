# blaster.py
#
# Open up a large number of socket connections with a server and then
# just start randomly blasting it with messages. 

from socket import *
import select
import random

NCONNECTIONS = 300                  # Number of connections to make
MSGSIZE      = 1024                 # Message size
SERVER       = ('localhost',45000)  # Server address
MSG          = "x"*MSGSIZE          # The message

# Open up connections
connections = []
for i in xrange(NCONNECTIONS):
    s = socket(AF_INET,SOCK_STREAM)
    s.connect(SERVER)
    connections.append(s)

# Send a message on a socket and get a response
def send_message(dest):
    bytes_sent = 0
    bytes_recv = 0
    while bytes_recv < MSGSIZE:
        r,w,e = select.select([dest],[dest],[])
        # Receive response data
        for s in r:
            bytes_recv += len(s.recv(65536))
        if bytes_sent < MSGSIZE:
            for s in w:
                bytes_sent += s.send(MSG[:MSGSIZE-bytes_sent])
    
# Send a bunch of random messages
def send_random(count):
    for x in xrange(count):
        dest = random.choice(connections)
        if x % 10000 == 0:
            print x
        send_message(dest)


# Run it
send_random(100000)

    
