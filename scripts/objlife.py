#!/usr/bin/env python

from struct import unpack
from si_prefix import si_format
from symbol import extract_func_symbols
import sys


def convert(file_path):
    objlist = []
    objmap = {}
    with open(file_path, 'rb') as f:
        depth = 0
        while f.read(1):
            f.seek(-1, 1)
            obj = unpack('l', f.read(8))[0]
            addr = unpack('L', f.read(8))[0]
            size = unpack('L', f.read(8))[0]
            time = unpack('L', f.read(8))[0]
            ctx = unpack('l', f.read(8))[0]
            ctx_addr = unpack('L', f.read(8))[0]

            if obj != -1:
                objlist.append([obj, addr, size, ctx, ctx_addr,\
                        -1, -1])
                objmap[addr] = obj
            else:
                obj = objmap[addr]
                objlist[obj][5:7] = [ctx, ctx_addr];

    objlist.sort(key = lambda tup: tup[2], reverse = True)
    return objlist


def print_objlist(objlist, func_table):
    for obj in objlist:
        objid, addr, size, alloc_ctx, alloc_ctx_addr, free_ctx, free_ctx_addr\
                = obj

        symbol = hex(alloc_ctx_addr)
        for (fstart, fend, fname) in func_table:
            if alloc_ctx_addr >= fstart and alloc_ctx_addr < fend:
                symbol = fname
                break

        prstring = str(objid).rjust(7)
        prstring += '   ' + hex(addr).rjust(14)
        prstring += '  (' + si_format(size).rjust(7) + 'B)'
        prstring += '   ' + str(alloc_ctx).rjust(8)
        prstring += '   ' + symbol
        print prstring


def main():
    if len(sys.argv) != 3:
        print "Usage: %s <binary file> <obj file>" % sys.argv[0]
        sys.exit(1)

    bin_file = sys.argv[1]
    obj_file = sys.argv[2]

    objlist = convert(obj_file)
    func_table = extract_func_symbols(bin_file)
    print_objlist(objlist, func_table)


if __name__ == "__main__":
    main()
