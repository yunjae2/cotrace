#!/usr/bin/env python

from struct import unpack
import sys


def convert(file_path):
    ctxinfo = {}
    with open(file_path, 'rb') as f:
        depth = 0
        while f.read(1):
            f.seek(-1, 1)
            ctx = unpack('i', f.read(4))[0]
            f.read(4)
            time = unpack('L', f.read(8))[0]

            if ctx not in ctxinfo:
                print "| " * depth + "ctx %d: %d" % (ctx, time)
                ctxinfo[ctx] = time
                depth = depth + 1
            else:
                depth = depth - 1
                print "| " * depth + "\____ %d" % (time)


def main():
    if len(sys.argv) != 2:
        print "Usage: %s <bin file path>" % sys.argv[0]
        sys.exit(1)

    file_path = sys.argv[1]

    convert(file_path)


if __name__ == "__main__":
    main()
