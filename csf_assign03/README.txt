Group members: 
    Cecelia Shuai: xshuai3@jh.edu
    Gigi Wang: ywang580@jhu.edu
_____________________________________________________________________________________________
Contribution:
    Worked on the some helper functions individually (equal amount of work), 
    and work on the main and most helper functions in person together. 
    Gigi also worked on the python code to do the profiling, and Cecelia 
    cleaned up the code and added the comments.
_____________________________________________________________________________________________
Report: 
We ran 20 experiments using gcc.trace to try different configurations regading
four different block sizes, two sets of parameter choices (NWA+WT, WA+WB), 
two eviction designs and three cache organizations (direct-mapped, set associative,
fully associative).

In specific, the four block sizes are 1, 32(smaller sizes), and 256 and 1024(larger sizes).
Two sets of parameter choices are 'no-write-allocate write-through' and
'write-allocate write-back'. And the two eviction designs are 'fifo' and 'lru'. 
We simulated the direct-mapped cache by setting the number of blocks to be 1, 
simulated the fully associative cache by setting the number of set to be 1, 
and simulated the set associative cahce by using other combinations of number of set/block.
In addition, we ensured the total data capacity to be constant (512KB = 2^19 bytes) 
so that their results are comparable.

Specificaly, with the 512KB capacity, three organizations can be simulated by 
                      num_set | num_block | byte_size 
    Direct Mapping      2^10        1          2^9      
    Direct Mapping      2^8        1           2^11     /* we change the num_set to see its effect on performance*/ 

    Fully Assocaitive    1         2^10        2^9 
    Fully Assocaitive    1         2^8         2^11     /* we change the num_block to see its effect*/ 

    Set Associative     2^5        2^5         2^9  

To separate the influences of cache organizations from the num_set and num_block, 
we changed these two stats within the same cache organization (commeneted by /*...*/ in the table above).
And on top of these organizations, we changed the NMA+WT vs. WA+WB and fifo vs. lru
to see their influences on the cache performances. Hence, we ended up with 20 experiments 
in total (5 x 2 x 2 = 20). And the detailed experimental results (statistics) are the following:
_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
Experiments results: 
Experiment 1
./csim 1 1024 512 write-allocate write-back lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317838
Load misses: 359
Store hits: 197073
Store misses: 413
Total cycles: 10396924


Experiment 2
./csim 1 1024 512 write-allocate write-back fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317838
Load misses: 359
Store hits: 197073
Store misses: 413
Total cycles: 10396924


Experiment 3
./csim 1 1024 512 no-write-allocate write-through lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317559
Load misses: 638
Store hits: 186137
Store misses: 11349
Total cycles: 28418696


Experiment 4
./csim 1 1024 512 no-write-allocate write-through fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317559
Load misses: 638
Store hits: 186137
Store misses: 11349
Total cycles: 28418696


Experiment 5
./csim 1 256 2048 write-allocate write-back lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317958
Load misses: 239
Store hits: 197317
Store misses: 169
Total cycles: 25757044


Experiment 6
./csim 1 256 2048 write-allocate write-back fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317922
Load misses: 275
Store hits: 197310
Store misses: 176
Total cycles: 29545808


Experiment 7
./csim 1 256 2048 no-write-allocate write-through lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317852
Load misses: 345
Store hits: 189037
Store misses: 8449
Total cycles: 37919489


Experiment 8
./csim 1 256 2048 no-write-allocate write-through fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317829
Load misses: 368
Store hits: 187415
Store misses: 10071
Total cycles: 39095444


Experiment 9
./csim 1024 1 512 write-allocate write-back lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317654
Load misses: 543
Store hits: 196927
Store misses: 559
Total cycles: 17782340


Experiment 10
./csim 1024 1 512 write-allocate write-back fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317654
Load misses: 543
Store hits: 196927
Store misses: 559
Total cycles: 17782340


Experiment 11
./csim 1024 1 512 no-write-allocate write-through lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317472
Load misses: 725
Store hits: 185743
Store misses: 11743
Total cycles: 29531815


Experiment 12
./csim 1024 1 512 no-write-allocate write-through fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317472
Load misses: 725
Store hits: 185743
Store misses: 11743
Total cycles: 29531815


Experiment 13
./csim 32 32 512 write-allocate write-back lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317836
Load misses: 361
Store hits: 197073
Store misses: 413
Total cycles: 10576122


Experiment 14
./csim 32 32 512 write-allocate write-back fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317829
Load misses: 368
Store hits: 197072
Store misses: 414
Total cycles: 10768115


Experiment 15
./csim 32 32 512 no-write-allocate write-through lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317557
Load misses: 640
Store hits: 186137
Store misses: 11349
Total cycles: 28444294


Experiment 16
./csim 32 32 512 no-write-allocate write-through fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317553
Load misses: 644
Store hits: 186137
Store misses: 11349
Total cycles: 28495490


Experiment 17
./csim 256 1 2048 write-allocate write-back lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317515
Load misses: 682
Store hits: 197137
Store misses: 349
Total cycles: 68099001


Experiment 18
./csim 256 1 2048 write-allocate write-back fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317515
Load misses: 682
Store hits: 197137
Store misses: 349
Total cycles: 68099001


Experiment 19
./csim 256 1 2048 no-write-allocate write-through lru < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317520
Load misses: 677
Store hits: 188758
Store misses: 8728
Total cycles: 54917278


Experiment 20
./csim 256 1 2048 no-write-allocate write-through fifo < trace/gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 317520
Load misses: 677
Store hits: 188758
Store misses: 8728
Total cycles: 54917278
_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
Dicussion of the best cache configuration: 
1. From the results, if we only change the cache organization, and keep other parameters the same,
we can compare direct mapping (DM) vs. fully associative (FA) vs. set associative (SA) by 
comparing Exp 1 vs. Exp 9 vs. Exp 13, and,  Exp 2 vs. Exp 10 vs. Exp v14, etc.
We observe that the DM resulted in higher load/store misses and much more cycles than FA anf SA,
FA and SA have comparable load/store hits/misses and SA has the least number of total cycles.
In this case, we consider SA to be a better organization than DM and FA at least for this experiment.

2. If we only change the NMA+WT vs. WA+WB, we can compare Exp1 vs. Exp3, Exp 2 vs. Exp4, etc.,
and we observe that NMA+WT resulted in higher load misses, significantly higher store misses
and total cyclers than WA+WB. Therefore, we consider WA+WB to be a better design for this experiment.

3. If we only change the fifo vs. lru, we can compare Exp1 vs. Exp2, Exp3 vs. Exp4, etc.,
and we observe that fifo usually resulted approximately the same or slightly higher load/store 
misses and total cycles. Therefore, we consider lru to be a better choice for this experiment.

4. If we only change the num_set, we can compare Exp 9 vs. Exp 17, Exp 10 vs. Exp 18, etc.,
and we observe that load/store misses/hits are comparable. However, the smaller num_set led to 
much higher total cycles. Therefore, we consider having larger num_set might be more desirable.

5. If we only change the num_block, we can compare Exp 1 vs. Exp 5, Exp 2 vs. Exp 6, etc., 
and we observe that higher num_block led to higher load/store misses but lower total 
cycles.
    Since this result is kind of ambiguous, we added a few additional experiments with num_set=32, 
    different num_blocks and byte sizes to keep capacity the same (512K), and with lru, WA+WB.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - -
    Here are the experiments:
    Experiment 21:
    ./csim 32 4 4096 write-allocate write-back lru < trace/gcc.trace
    Total loads: 318197
    Total stores: 197486
    Load hits: 317974
    Load misses: 223
    Store hits: 197358
    Store misses: 128
    Total cycles: 47312260
    
    Experiment 22:
    ./csim 32 16 1024 write-allocate write-back lru < trace/gcc.trace
    Total loads: 318197
    Total stores: 197486
    Load hits: 317921
    Load misses: 276
    Store hits: 197229
    Store misses: 257
    Total cycles: 15030607

    Experiment 23: 
    ./csim 32 256 64 write-allocate write-back lru < trace/gcc.trace
    Total loads: 318197
    Total stores: 197486
    Load hits: 317204
    Load misses: 993
    Store hits: 195112
    Store misses: 2374
    Total cycles: 5901890

    Experiment 24:
    ./csim 32 1024 16 write-allocate write-back lru < trace/gcc.trace
    Total loads: 318197
    Total stores: 197486
    Load hits: 315855
    Load misses: 2342
    Store hits: 188617
    Store misses: 8869
    Total cycles: 4997741
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - -
And we could observe that from Exp 21 to 24, as the num_block is increasing, the number of total
cycles drops significantly, but load/store misses also increases significantly, maintaining the 
same trend as previous experiments show. Therefore, we conclude that higher number of blocks leads 
to lower total cycles, higher load/store misses. The best plan, therefore, is to choose an intermediate
number (not too high or too low) so that we can maintain a reasonable amount of load hits and store hits, 
while also decreasing the total cycles to some extent. 

Since we are looking for the best cache configuration, we want higher hit rate (lower number 
of misses), and fewer cycles and miss penalties. And from our experiments, we can conclude that
set associative organization, WA+WB, lru and an intermediate number of num_block are more desirable. 