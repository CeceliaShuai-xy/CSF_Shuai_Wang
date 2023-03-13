#include <iostream>
#include <string>
#include <cstdlib>
<<<<<<< HEAD
#include <exception>
#include "csim_functions.h"
=======
#include <sstream>
>>>>>>> 5c8933ef3e72d49d49a252c8e5a7dab4c8db8aee

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::getline;
using std::string;
using std::atoi;
<<<<<<< HEAD
using std::stoi;
using std::exception;
=======
using std::stringstream;
>>>>>>> 5c8933ef3e72d49d49a252c8e5a7dab4c8db8aee

int main(int argc, char* argv[]) {

    if (!(argc == 7)) {
        cerr << "Error: invalid number of command line arguments." << endl;
        return 1;
    }
    Input_parm input_parm;

    try {
        input_parm.num_sets = stoi(argv[1]);
        input_parm.num_blocks = stoi(argv[2]);
        input_parm.num_bytes = stoi(argv[3]);
        input_parm.allocate = argv[4];
        input_parm.write_command = argv[5];
        input_parm.evictions = argv[6];
    } catch (exception &ex) {
        cerr << "Error: command line arguments conversion failure." << endl;
    }

    try {
    checkValidAuguments(input_param)
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

<<<<<<< HEAD
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
=======
    cout << n_set << " " << n_block << " " << n_byte << endl;
    cout << allocate << " " << write_command << " " << evictions << endl;

    string n; 
    uint32_t address;
    string load_write;
    string memory;

    while (getline(cin,n)) {
        stringstream ss(n);
        ss >> load_write;
        ss >> memory;
        address = std::stoul(memory, 0, 16);

        // cout << "This is a new line" << endl;
        // cout << "l or w: " << load_write << endl;
        // cout << "memory addr: " << hex_num << endl;
    }
>>>>>>> 5c8933ef3e72d49d49a252c8e5a7dab4c8db8aee


    return 0;

    
}