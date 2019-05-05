#!/usr/bin/env python

from __future__ import division
from termcolor import colored
from struct import unpack
import sys


def convert(file_path, start_ctx, max_depth):
    ctxidx = {}
    ctxtree = []
    with open(file_path, 'rb') as f:
        depth = 0
        report_on = False
        cidx = 0
        nr_ctx = 0
        while f.read(1):
            f.seek(-1, 1)
            ctx = unpack('i', f.read(4))[0]
            f.read(4)
            addr = unpack('L', f.read(8))[0]
            time = unpack('L', f.read(8))[0]

            if not report_on:
                if ctx == start_ctx:
                    report_on = True
                else:
                    continue

            if ctx not in ctxidx:
                if depth > max_depth:
                    continue
                ctxidx[ctx] = nr_ctx
                ctxtree.append([ctx, addr, time, -1, depth])
                nr_ctx = nr_ctx + 1
                depth = depth + 1
            else:
                cidx = ctxidx[ctx]
                ctxtree[cidx][3] = time
                depth = depth - 1
                if ctx == start_ctx:
                    break

    return ctxtree


def print_ctxtree(ctxtree, max_depth):
    time_total = [[]] * (max_depth + 1)
    print "#  <elapsed time>        <context tree>"
    for ctxline in ctxtree:
        ctx, addr, start, end, depth = ctxline
        time = end - start
        time_total[depth] = time

        prstring = (str(time / 1000) + " us").rjust(12) + " "
        parent_depth = depth - 1
        if depth == 0:
            parent_depth = 0
        time_percent = time / time_total[parent_depth] * 100

        percent_string = "(" + ("%3.2f" % time_percent).rjust(6) + " %)"
        if depth > 1:
            prstring += colored(percent_string, 'green', attrs=['dark'])
        else:
            prstring += percent_string

        prstring += "  "
        if depth:
            prstring += "    " + "|   " * (depth - 1)
        prstring += str(ctx) + "    " + hex(addr)

        print prstring


def main():
    if len(sys.argv) != 4:
        print "Usage: %s <bin file path> <start ctx> <max_depth>" % sys.argv[0]
        sys.exit(1)

    file_path = sys.argv[1]
    start_ctx = int(sys.argv[2])
    max_depth = int(sys.argv[3])

    ctxtree = convert(file_path, start_ctx, max_depth)
    print_ctxtree(ctxtree, max_depth)


if __name__ == "__main__":
    main()
