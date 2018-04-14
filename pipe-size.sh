#!/bin/bash

make pipe-size

echo "Max pipe size for unprivileged user:"
cat /proc/sys/fs/pipe-max-size

echo

echo "Test with pipe"
echo "testdata" | ./pipe-size

echo

echo "Test with fifo"
mkfifo fifo
./pipe-size fifo &
echo "testdata" > fifo

rm fifo
