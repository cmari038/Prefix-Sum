#!/usr/bin/env python3
import os
import re
from subprocess import check_output
from time import sleep

import matplotlib.pyplot as plt

#
#  Feel free (a.k.a. you have to) to modify this to instrument your code
#

# list(range(2,32,2))
# ["seq_64_test.txt"]
#  ["1k.txt","8k.txt","16k.txt"]
SPIN = True
THREADS = list(range(0,34,2))
LOOPS = [10]
INPUTS =  ["1k.txt","8k.txt","16k.txt"]

csvs = []
for inp in INPUTS:
    for loop in LOOPS:
        csv = ["{}/{}".format(inp, loop)]
        for thr in THREADS:
            cmd = "./bin/prefix_scan -o temp.txt -n {} -i tests/{} -l {} -s {}".format(
                thr, inp, loop, SPIN)
            out = check_output(cmd, shell=True).decode("ascii")
            m = re.search("time: (.*)", out)
            if m is not None:
                time = m.group(1)
                csv.append(time)

        csvs.append(csv)
        sleep(0.5)

header = ["microseconds"] + [str(x) for x in THREADS]

print("\n")
print(", ".join(header))
for csv in csvs:
    print (", ".join(csv))

oneK = csvs[0]
eightK = csvs[1]
sixteenK = csvs[2]


for t in range(len(csvs[0])):
    if t == 0 or t == 1:
        continue
    else:
        oneK[t] = int(oneK[1]) / int(oneK[t])
        eightK[t] = int(eightK[1]) / int(eightK[t])
        sixteenK[t] = int(sixteenK[1]) / int(sixteenK[t])

plt.clf()
plt.plot(header[2:], oneK[2:], label='1K')
plt.plot(header[2:], eightK[2:], label='8K')
plt.plot(header[2:], sixteenK[2:], label='16K')
plt.legend()
plt.show()