"""
    Author: Allen B. Downey

    This file contains an example program from The Little Book of
    Semaphores, available from Green Tea Press, greenteapress.com

    This code is in the public domain.
"""


import threading, time, os, signal, sys

class Thread(threading.Thread):
    """Wrapper for the Thread class in the threading module."""

    def __init__(self, target, *args):
        """Create and start a thread.

        target: callable
        args: passed along to target
        """
        threading.Thread.__init__(self, target=target, args=args)
        self.start()


class Semaphore(threading._Semaphore):
    """Wrapper for the Semaphore class in the threading module."""
    wait = threading._Semaphore.acquire
    
    def signal(self, n=1):
        """Signal the semaphore.

        n: how many times to signal
        """
        for i in range(n):
            self.release()

    def value(self):
        """Returns the current value of the semaphore.

        Note: asking for the value of a semaphore is almost always
        a bad idea.  If you do anything based on the result, it is
        likely to be a mistake.
        """
        return self._Semaphore__value

    
def watcher():
    """Forks a process, and the child process returns.

    The parent process waits for a KeyBoard interrupt, kills
    the child, and exits.

    This is a workaround for a problem with Python threads:
    when there is more than one thread, a KeyBoard interrupt
    might be delivered to any of them (or occasionally, it seems,
    none of them).
    """
    child = os.fork()
    if child == 0:
        return

    try:
        os.wait()
    except KeyboardInterrupt:
        print 'KeyBoardInterrupt'
        try:
            os.kill(child, signal.SIGKILL)
        except OSError:
            pass
    sys.exit()


