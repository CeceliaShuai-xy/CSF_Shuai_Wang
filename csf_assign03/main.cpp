/*
 * Main function to simulate cache on a variety of traces
 * CSF Assignment 3 
 * Cecelia Shuai 
 * xshuai3@jh.edu
 * Gigi Wang 
 * ywang580@jhu.edu
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#include "csim_functions.h"
#include <sstream>

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::getline;
using std::string;
using std::atoi;
using std::stoi;
using std::exception;
using std::stringstream;

/*
 * Main function to simulate cache based on the input params.
 * Handle exceptions when the input arguments are invalid,
 * and print stats regarding cache performance.
 *
 * Parameters:
 *   arg - number of argument
 *   argv - argument 
 *
 * Returns:
 *   0 - when no error pops up
 *   nonzero - otherwise
 */
int main(int argc, char* argv[]) {

    if (!(argc == 7)) {
        cerr << "Error: invalid number of command line arguments." << endl;
        return 1;
    }
    Input_param input_param;

    try {
        input_param.num_sets = stoi(argv[1]);
        input_param.num_blocks = stoi(argv[2]);
        input_param.num_bytes = stoi(argv[3]);
        input_param.allocate = argv[4];
        input_param.write_command = argv[5];
        input_param.evictions = argv[6];
    } catch (exception &ex) {
        cerr << "Error: command line arguments conversion failure." << endl;
        return 2;
    }

    try {
        checkValidAuguments(&input_param);
    } catch (exception &ex) {
        cerr << ex.what() << endl;
        return 3;
    }
    
    Cache cache = initialization_cache(input_param);
    Stats stats = initialization_stats();

    uint32_t offset_pos = log2(input_param.num_bytes);
    uint32_t index_pos = log2(input_param.num_sets) + offset_pos;

    string n; 
    uint32_t address;
    string load_write;
    string memory;

    while (getline(cin,n)) {
        stringstream ss(n);
        ss >> load_write;
        ss >> memory;
        address = std::stoul(memory, 0, 16);

        // get tag and index
        uint32_t tag = findTag(address, index_pos);
        uint32_t index = findIndex(address, offset_pos, index_pos);

        // check load/write 
        if (load_write == "l") {
            load_cache(&cache, tag, index, &stats, &input_param);
        } else {
            save_cache(&cache, tag, index, &stats, &input_param);
        }
        cache.current_max++;
    }

    printStats(stats);
    return 0;
}

// Pseudo code for implementation of cache
/*
current_max = 0
while (user input) {
    
    tag = findTag
    index = findIndex
    if load:
        if hit:
            if lru:
                update access_ts
            total_cycle ++;
        not hit:
            if has invalid:
                put slot in
            if no invalid:
                replace according lru/fifo
    if save:
        if hit:
            if lru:
                update access_ts
            if write_throght:
                total_cycle 101
            if write_back:
                dirty block
                total_cycle ++
        not hit:
            if write-allocate:
                if has invalid:
                    put slot in
                if no invalid:
                    replace according lru/fifo
            if no write allocate:
                total_cycle 100

    cache.current_max ++;
}
*/