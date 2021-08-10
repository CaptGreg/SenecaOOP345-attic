#!/usr/bin/python
# Copyright (c) 2001-2004 Twisted Matrix Laboratories.
# See LICENSE for details.

#

from twisted.internet.protocol import Protocol, Factory
from twisted.internet import reactor

### Protocol Implementation

# This is just about the simplest possible protocol
class Echo(Protocol):
    def connectionMade(self):
        print "Got a connection"

    def dataReceived(self, data):
        """As soon as any data is received, write it back."""
        self.transport.write(data)


def main():
    f = Factory()
    f.protocol = Echo
    reactor.listenTCP(45000, f)
    reactor.run()

if __name__ == '__main__':
    main()
