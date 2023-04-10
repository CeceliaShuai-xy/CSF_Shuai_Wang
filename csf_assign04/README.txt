CONTRIBUTIONS
Group members:      
    Cecelia Shuai xshuai3
    Gigi Wang ywang580
Both members contributed equally to the homework by working together in person to 
brainstorm and write the code together.

REPORT
Experiments:
        Wrote 16777216 bytes to '/tmp/xshuai3/data_16M.in'
        Test run with threshold 2097152

        real    0m0.344s
        user    0m0.328s
        sys     0m0.014s
        Test run with threshold 1048576

        real    0m0.208s
        user    0m0.360s
        sys     0m0.016s
        Test run with threshold 524288

        real    0m0.166s
        user    0m0.428s
        sys     0m0.030s
        Test run with threshold 262144

        real    0m0.128s
        user    0m0.541s
        sys     0m0.044s
        Test run with threshold 131072

        real    0m0.133s
        user    0m0.504s
        sys     0m0.055s
        Test run with threshold 65536

        real    0m0.132s
        user    0m0.514s
        sys     0m0.071s
        Test run with threshold 32768

        real    0m0.132s
        user    0m0.511s
        sys     0m0.101s
        Test run with threshold 16384

        real    0m0.142s
        user    0m0.546s
        sys     0m0.115s

Explanation: 
When we decreased the threshold, we first saw the decrement of real time, which is the 
amount of time that elapsed between when the program started and exited. This is because as
we had smaller threshold, we created more child processes to run the qsort in a smaller array 
in parallel. Since qsort is O(N^2), having a smaller array will speed up the processes.
However, as the threshold decreases to, for instance, 131072, we observed an increase in
real time. This might due to that the merging part needs to make a copy of the sub-arrays
from the child processes, which is (O(N)) and consumes a considerable amound of time.

Every time we call mergesort, if the length of the array is above the threshold, 
we will fork a child process to sort the left half of the array and the parent will then 
sort the right half of the array in parallel. And this call is recursive and will start 
merging when the left and right part of the array are sorted. 

The merge sort of each half of the array can be scheduled by the OS kernel in parallel in 
different CPU cores. Theoretically, we expect the left and right half of the arrays are 
sorted in parallel, but due to the limitatino of how many processes can be run at the same 
time, some child processes might need to wait for the others to finish first.
As a result, the left might need to wait for the right to be sorted to be
eventually merged together, and vice versa.
