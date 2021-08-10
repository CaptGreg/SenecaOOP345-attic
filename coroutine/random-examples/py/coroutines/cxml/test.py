import cxmlparse

def coroutine(func):
    def start(*args,**kwargs):
        cr = func(*args,**kwargs)
        cr.next()
        return cr
    return start

@coroutine
def buses_to_dicts(target):
    while True:
        event, value = (yield)
        if event == 'start' and value[0] == 'bus':
            busdict = { }
            fragments = []
            # Capture inner elements of <bus> elements
            while True:
                event, value = (yield)
                if event == 'start':   fragments = []
                elif event == 'text':  fragments.append(value)
                elif event == 'end':
                    if value != 'bus': 
                        busdict[value] = "".join(fragments)
                    else:
                        target.send(busdict)
                        break

@coroutine
def filter_on_field(fieldname,value,target):
    while True:
        d = (yield)
        if d.get(fieldname) == value:
            target.send(d)

@coroutine
def bus_locations():
    while True:
        bus = (yield)
        print "%(route)s,%(id)s,\"%(direction)s\","\
              "%(latitude)s,%(longitude)s" % bus 

@coroutine
def printer():
    while True:
        item = (yield)
        print item


cxmlparse.parse("../allroutes.xml",
        buses_to_dicts(
        filter_on_field("route","22",
        filter_on_field("direction","North Bound",
        bus_locations()))))
