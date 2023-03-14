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
    }

    try {
        checkValidAuguments(input_param);
    } catch (exception &ex) {
        // print error message
        return 1;
    }
    
    Cache cache = initialization_cache(input_param);
    Stats stats = initialization_stats();

    uint32_t offset_pos = log2(input_param.num_bytes);
    uint32_t index_pos = log2(input_param.num_sets) + offset_pos;


    // checking invalid cache parameters
    // if ((n_set <= 0) || (n_set % 2 != 0)) {
    //     cerr << "invalid number of sets." << endl;
    //     return 2;
    // }
    // if ((n_block <= 0) || (n_block % 2 != 0)) {
    //     cerr << "invalid number of blocks." << endl;
    //     return 3;
    // }


    // if ((n_byte < 4) || (n_byte % 2 != 0)) {
    //     cerr << "invalid number of bytes." << endl;
    //     return 4;
    // }

    // if ((allocate != "write-allocate") && (allocate != "no-write-allocate")) {
    //     cerr << "invalid allocation method." << endl;
    //     return 5;
    // }

    // if ((write_command != "write-through") && (write_command != "write-back")) {
    //     cerr << "invalid writing method." << endl;
    //     return 6;
    // }

    // if ((evictions != "lru") && (evictions != "fifo")) {
    //     cerr << "invalid evictions" << endl;
    // }

    // cout << n_set << " " << n_block << " " << n_byte << endl;
    // cout << allocate << " " << write_command << " " << evictions << endl;

    // string n; 
    // int count = 0;
    // int hex_num;
    // while (cin >> n) {
    //     if (count == 1) {
    //         hex_num = std::stoul(n, 0, 16);
    //     }
    //     if (count == 2) {
    //         count = 0;
    //         continue;
    //     }
    //     count++;
    // }

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
    printStats(stats);
    return 0;
}