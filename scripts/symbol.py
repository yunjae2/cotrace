import subprocess
import re

def extract_func_symbols(bin_file):
    func_table = []

    cmd = "nm -nS --defined-only %s" % bin_file
    res = subprocess.check_output(cmd, shell=True, executable="/bin/bash")
    lines = re.split('\n', res)

    for line in lines:
        fields = line.strip().split()
        if len(fields) != 4:
            continue
        if fields[2] != 'T' and fields[2] != 't':
            break
        addr = int(fields[0], 16)
        size = int(fields[1], 16)
        symbol = fields[3]
        func_table.append([addr, addr + size, symbol])

    return func_table
