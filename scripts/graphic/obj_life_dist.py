#!/usr/bin/env python
import sys
import re
import numpy as np
import math
import matplotlib.pyplot as plt
from si_prefix import si_parse
from matplotlib.ticker import EngFormatter


def get_alloc_sizes(objdata):
    lifetimes = []
    nonfreed = 0
    with open(objdata, 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith('<'):
                continue

            fields = re.split(' *\( *| *\) *| +', line)
            lifetime = fields[4]
            if lifetime == 'INF':
                nonfreed = nonfreed + 1
                continue
            lifetimes.append(float(lifetime))
    return lifetimes, nonfreed


def draw_histogram(vals, spnum):
    fig, ax = plt.subplots()
    formatter = EngFormatter(unit = 'B')
    n, bins, patches = ax.hist(vals)
    plt.yscale('symlog')
    plt.xlabel("lifetime of objects")
    plt.ylabel("number of objects")
    plt.title("Total number of objects: " + str(spnum + len(vals)) + "\nobjects not freed: " + str(spnum))
    plt.show()


def main():
    if len(sys.argv) < 2 or sys.argv[1] == '-h':
        print "usage: %s <output of objlist>" % sys.argv[0]
        sys.exit()
    datapath = sys.argv[1]
    lifetimes, nonfreed = get_alloc_sizes(datapath)
    draw_histogram(lifetimes, nonfreed)


if __name__ == "__main__":
    main()
