#!/usr/bin/env python

from __future__ import division
from termcolor import colored
from struct import unpack
from symbol import extract_func_symbols
import sys


def extract_ctxlist(file_path):
    ctxlist = []
    with open(file_path, 'rb') as f:
        while f.read(1):
            f.seek(-1, 1)
            ctx = unpack('l', f.read(8))[0]
            addr = unpack('L', f.read(8))[0]
            start_time = unpack('L', f.read(8))[0]
            end_time = unpack('L', f.read(8))[0]
            depth = unpack('i', f.read(4))[0]
            f.read(4)

            ctxlist.append([ctx, addr, start_time, end_time, depth])

    print "root ctx: " + str(ctxlist[-1][0])
    return ctxlist


def convert(file_path, start_ctxid, max_depth):
    ctxidx = {}
    ctxtree = []
    ctxlist = extract_ctxlist(file_path)
    ctxlist.sort(key = lambda tup: tup[2])

    report_on = False
    start_depth = -1
    for ctx in ctxlist:
        ctxid, addr, start_time, end_time, depth = ctx

        if depth <= start_depth:
            break

        if not report_on:
            if ctxid == start_ctxid:
                report_on = True
                start_depth = depth
            else:
                continue

        if (depth - start_depth) > max_depth:
            continue

        ctxtree.append([ctxid, addr, start_time, end_time, depth - start_depth])

    return ctxtree


def print_ctxtree(ctxtree, func_table, max_depth):
    time_total = [[]] * (max_depth + 1)
    print "#  <elapsed time>        <context tree>"
    for ctxline in ctxtree:
        ctx, addr, start, end, depth = ctxline
        time = end - start
        time_total[depth] = time

        symbol = hex(addr)
        for (fstart, fend, fname) in func_table:
            if addr >= fstart and addr < fend:
                symbol = fname
                break

        prstring = ("%.3f" % (time / (1000 * 1000 * 1000)) + " s").rjust(12) + " "
        parent_depth = max(depth - 1, 0)
        time_percent = time / time_total[parent_depth] * 100

        percent_string = "(" + ("%3.2f" % time_percent).rjust(6) + " %)"
        if depth > 1:
            percent_string = colored(percent_string, 'green', attrs=['dark'])
        prstring += percent_string

        prstring += "  "
        if depth:
            prstring += "    " + "|    " * (depth - 1)
        prstring += str(ctx).ljust(4) + "    " + symbol

        print prstring


def main():
    if len(sys.argv) != 5:
        print "Usage: %s <binary file> <ctx file> <start ctx> <max depth>" \
            % sys.argv[0]
        sys.exit(1)

    bin_file = sys.argv[1]
    ctx_file = sys.argv[2]
    start_ctx = int(sys.argv[3])
    max_depth = int(sys.argv[4])

    ctxtree = convert(ctx_file, start_ctx, max_depth)
    func_table = extract_func_symbols(bin_file)
    print_ctxtree(ctxtree, func_table, max_depth)


if __name__ == "__main__":
    main()
