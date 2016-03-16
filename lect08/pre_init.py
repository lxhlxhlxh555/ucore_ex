#! /usr/local/bin/python3
import os

import sys

if __name__ == "__main__":

    if (len(sys.argv)) < 3:
        print("please input fin_path fout_path")
        exit(-1)

    fin_path = sys.argv[1]
    fout_path = sys.argv[2]
    with open(fin_path, "r") as fin:
        with open(fout_path, "wb") as fout:
            lines = fin.readlines()
            for line in lines:
                bb = line.split('\n')[0].split(': ')[1].split(' ')
                for b in bb:
                    fout.write(int(b, 16).to_bytes(1, 'little'))
