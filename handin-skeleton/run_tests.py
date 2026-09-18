#!/usr/bin/env python3
import os
import re
from subprocess import check_output
from time import sleep

#import matplotlib.pyplot as plt

#
#  Feel free (a.k.a. you have to) to modify this to instrument your code
#

"""def graph(csvs, header):
    oneK = csvs[0]
    eightK = csvs[1]
    sixteenK = csvs[2]

    for t in range(len(csvs[0])):
        if t == 0 or t == 1:
            continue
        else: 
            oneK[t] = float(oneK[1]) / float(oneK[t])
            eightK[t] = float(eightK[1]) / float(eightK[t])
            sixteenK[t] = float(sixteenK[1]) / float(sixteenK[t])

    plt.clf()
    plt.plot(header[2:], oneK[2:], label='15 loops')
    plt.plot(header[2:], eightK[2:], label='30 loops')
    plt.plot(header[2:], sixteenK[2:], label='40 loops')
    plt.legend()
    plt.title("Inflection Point with Spin")
    plt.xlabel("Threads")
    plt.ylabel("Seq Time / Parallel Time")
    plt.show()"""
    

# list(range(0,34,2))
# ["seq_64_test.txt"]
#  ["1k.txt","8k.txt","16k.txt"]
SPIN = False
THREADS = list(range(0,34,2))
LOOPS = [10]
INPUTS = ["1k.txt","8k.txt","16k.txt"]

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


#graph(csvs, header)