# runblaster.py

import subprocess
import time

NPROCS = 3

start = time.time()

procs = []
for i in xrange(NPROCS):
    p = subprocess.Popen(['python','blaster.py'])
    procs.append(p)

for p in procs:
    p.wait()

end = time.time()

print end-start
    
