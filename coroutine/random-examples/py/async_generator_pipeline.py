import asyncio
import time

tasks = []

def func(targets, task_id, param):
    print(f"{task_id}: Initialised with param: {param}")

    async def func_inner(targets, inpt):
        await asyncio.sleep(1)  # simulated IO delay
        print(f"{task_id}: Received input: {inpt}")
        outp = inpt
        for target in targets or []:
            print(f"{task_id}: T1 sending {outp}")
            target.send(outp)

    while True:
        inpt = (yield)
        print(f'{task_id}: Creating task with {task_id}_inner, input {inpt}.')
        tasks.append(asyncio.create_task(func_inner(targets, inpt)))


async def main():
    gen2 = func([], 'T2', "hello T2")
    gen2.send(None)

    gen1 = func([gen2], 'T1', "hello T1")
    gen1.send(None)

    start_time = time.time()
    gen1.send(1)
    gen1.send(2)
    gen1.send(3)

    await asyncio.gather(*tasks)
    await asyncio.gather(*tasks)  # round 2
    print(f"Duration: {time.time() - start_time}")

asyncio.run(main())