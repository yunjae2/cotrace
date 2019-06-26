#!/usr/bin/env python
import sys
import re
import numpy as np
import math
import matplotlib.pyplot as plt
from si_prefix import si_parse
from matplotlib.ticker import EngFormatter


def get_alloc_sizes_alloctrace(objdata):
    sizes = []
    with open(objdata, 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith('#'):
                continue

            fields = line.split()
            size = int(fields[2])
            sizes.append(size)

    return sizes


def get_alloc_sizes(objdata):
    sizes = []
    with open(objdata, 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith('<'):
                continue

            fields = re.split(' *\( *| *\) *| +', line)
            sz_w_sfx = fields[2] + fields[3]
            size = si_parse(sz_w_sfx[0:-1])
            sizes.append(size)
    return sizes


def draw_histogram(vals):
    maxval = max(vals)
    fig, ax = plt.subplots()
    formatter = EngFormatter(unit = 'B')
    n, bins, patches = ax.hist(vals,
            bins = 10**np.linspace(0, math.ceil(math.log10(maxval)), math.ceil(math.log10(maxval)) + 1))
    plt.yscale('symlog')
    plt.xscale('symlog')
    ax.xaxis.set_major_formatter(formatter)
    plt.xlabel("allocation size")
    plt.ylabel("number of allocations")
    plt.title("Total number of allocations: " + str(len(vals)))
    plt.show()


def main():
    if len(sys.argv) < 2 or sys.argv[1] == '-h':
        print "usage: %s <output of objlist>" % sys.argv[0]
        sys.exit()
    if len(sys.argv) > 2 and sys.argv[1] == 'alloctrace':
        datapath = sys.argv[2]
        sizes = get_alloc_sizes_alloctrace(datapath)
    else:
        datapath = sys.argv[1]
        sizes = get_alloc_sizes(datapath)
    draw_histogram(sizes)


if __name__ == "__main__":
    main()
