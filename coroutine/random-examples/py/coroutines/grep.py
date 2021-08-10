# grep.py
#
# A very simple coroutine

def grep(pattern):
    print "Looking for %s" % pattern
    while True:
        line = (yield)
        if pattern in line:
            print line,

# Example use
if __name__ == '__main__':
    g = grep("python")
    g.next()
    g.send("Yeah, but no, but yeah, but no")
    g.send("A series of tubes")
    g.send("python generators rock!")
