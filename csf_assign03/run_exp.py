from cProfile import run
import os
import itertools


# below are parameters to vary:

sets_size = [1, 1024]
block_size = [1, 4, 1024]
bytes_size = [16]
write = ["write-allocate write-back", "no-write-allocate write-through"]
eviction = ["lru", "fifo"]

list_ = [sets_size, block_size, bytes_size, write, eviction]

combination = [p for p in itertools.product(*list_)]

for i, comb in enumerate(combination):
    print('\n')
    run_string = f'./csim {comb[0]} {comb[1]} {comb[2]} {comb[3]} {comb[4]} < trace/gcc.trace'
    print(f'Experiment {i+1}')
    print(run_string)
    os.system(run_string)
