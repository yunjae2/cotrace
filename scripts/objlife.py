#!/usr/bin/env python

from struct import unpack
import sys


def convert(file_path):
    objinfo = []
    objmap = {}
    with open(file_path, 'rb') as f:
        depth = 0
        while f.read(1):
            f.seek(-1, 1)
            obj = unpack('i', f.read(4))[0]
            f.read(4)
            addr = unpack('L', f.read(8))[0]
            size = unpack('L', f.read(8))[0]
            time = unpack('L', f.read(8))[0]

            if obj != -1:
                objinfo.append([addr, size, time, -1])
                objmap[addr] = obj
                print '[%10d]\tobj %2d alloc: %10d (%d)' % (time, obj, addr, size)
            else:
                obj = objmap[addr]
                objinfo[obj][3] = time;
                size = objinfo[obj][1]
                print '[%10d]\tobj %2d  free: %10d (%d)' % (time, obj, addr, size)


def main():
    if len(sys.argv) != 2:
        print "Usage: %s <bin file path>" % sys.argv[0]
        sys.exit(1)

    file_path = sys.argv[1]

    convert(file_path)


if __name__ == "__main__":
    main()
