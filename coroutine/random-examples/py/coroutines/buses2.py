# buses.py
#
# An example of setting up an event handling pipeline with coroutines
# and XML parsing.

from coroutine import *

@coroutine 
def text_collector(target):
    while True:
        while True:
            event, value = (yield)
            if event == 'start' or event == 'end':
                target.send((event,value))
            else:
                chunks = [value]
                while True:
                    event, value = (yield)
                    if event != 'text': break
                    chunks.append(value)
                target.send(('text',"".join(chunks)))
                target.send((event,value))
                            
@coroutine
def buses_to_dicts(target):
    while True:
        event, value = (yield)
        # Look for the start of a <bus> element
        if event == 'start' and value[0] == 'bus':
            busdict = { }
            # Capture text of inner elements in a dict
            while True:
                event, value = (yield)
                if event == 'text':  
                    textvalue = value
                elif event == 'end':
                    if value != 'bus':
                        busdict[value] = textvalue
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

# Example 
if __name__ == '__main__':
    import xml.sax
    from cosax import EventHandler

    xml.sax.parse("allroutes.xml",
              EventHandler(
                   text_collector(
                   buses_to_dicts(
                   filter_on_field("route","22",
                   filter_on_field("direction","North Bound",
                   bus_locations()))))
              ))
