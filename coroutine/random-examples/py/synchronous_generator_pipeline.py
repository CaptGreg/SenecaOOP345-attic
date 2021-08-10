import time

def func(targets, task_id, param=None):
    print(f"{task_id}: Initialised with param: {param}")
    while True:
        inpt = (yield)
        print(f"{task_id}: Received input: {inpt}")
        time.sleep(1)  # simulated IO delay
        for target in targets:
            print(f"{task_id}: T1 sending {inpt}")
            target.send(inpt)

gen2 = func([], 'T2',  "hello T2")
gen2.send(None)

gen1 = func([coro2], 'T1',  "hello T1")
gen1.send(None)

start_time = time.time()
gen1.send(1)
gen1.send(2)
gen1.send(3)
print(f"Duration: {time.time() - start_time}")