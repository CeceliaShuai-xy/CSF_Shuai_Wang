import os
import itertools
import numpy as np


# below are parameters to vary:

sets_size = [1, 1024, 32, 256]
block_size = [1, 1024, 32, 256]
bytes_size = [512, 2048]
write = ["write-allocate write-back", "no-write-allocate write-through"]
eviction = ["lru", "fifo"]
capacity = 19 # power of 2

list_ = [sets_size, block_size, bytes_size, write, eviction]

combination = [p for p in itertools.product(*list_)]

exp_num = 1
for i, comb in enumerate(combination):
    cap = np.log2(comb[0]) + np.log2(comb[1]) + np.log2(comb[2])
    if int(cap) != capacity:
        continue
    print('\n')
    run_string = f'./csim {comb[0]} {comb[1]} {comb[2]} {comb[3]} {comb[4]} < trace/gcc.trace'
    print(f'Experiment {exp_num}')
    print(run_string)
    os.system(run_string)
    exp_num += 1
