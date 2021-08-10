# echo1.py
#
# A another attempt at an echo server

from socket import *

def handle_client(client,addr):
    print "Connection from", addr
    while True:
        yield ReadWait(client)
        data = client.recv(8192)
        if not data:
            break
        yield WriteWait(client)
        client.send(data)
    client.close()
    print "Client closed"

def server(port):
    print "Server starting"
    sock = socket(AF_INET,SOCK_STREAM)
    sock.bind(("",port))
    sock.listen(5)
    while True:
        yield ReadWait(sock)
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
