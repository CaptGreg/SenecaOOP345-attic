# task.py
#
# A simple object representing a "Task"

class Task(object):
    taskid = 0
    def __init__(self,target):
        Task.taskid += 1
        self.tid     = Task.taskid   # Task ID
        self.target  = target        # Target coroutine
        self.sendval = None          # Value to send
    def run(self):
        return self.target.send(self.sendval)

# An example showing the control flow
if __name__ == '__main__':
    
    # A simple generator/coroutine function
    def foo():
        print "Part 1"
        yield
        print "Part 2"
        yield

    t1 = Task(foo())
    print "Running foo()"
    t1.run()
    print "Resuming foo()"
    t1.run()

    # If you call t1.run() one more time, you get StopIteration.
    # Uncomment the next statement to see that.

    # t1.run()

