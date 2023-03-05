#include <iostream>
#include <string>
#include <cstdlib>

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::getline;
using std::string;
using std::atoi;

int main(int argc, char* argv[]) {

    if (!(argc == 7)) {
        cerr << "invalid number of command line arguments.";
        return 1;
    }

    int n_set = atoi(argv[1]);
    int n_block = atoi(argv[2]);
    int n_byte = atoi(argv[3]);
    string allocate = argv[4];
    string write_command = argv[5];
    string evictions = argv[6];

    // checking invalid cache parameters
    if ((n_set <= 0) || (n_set % 2 != 0)) {
        cerr << "invalid number of sets." << endl;
        return 2;
    }
    if ((n_block <= 0) || (n_block % 2 != 0)) {
        cerr << "invalid number of blocks." << endl;
        return 3;
    }

    if ((n_byte < 4) || (n_byte % 2 != 0)) {
        cerr << "invalid number of bytes." << endl;
        return 4;
    }

    if ((allocate != "write-allocate") && (allocate != "no-write-allocate")) {
        cerr << "invalid allocation method." << endl;
        return 5;
    }

    if ((write_command != "write-through") && (write_command != "write-back")) {
        cerr << "invalid writing method." << endl;
        return 6;
    }

    if ((evictions != "lru") && (evictions != "fifo")) {
        cerr << "invalid evictions" << endl;
    }

    cout << n_set << " " << n_block << " " << n_byte << endl;
    cout << allocate << " " << write_command << " " << evictions << endl;

    string n; 
    int count = 0;
    int hex_num;
    while (cin >> n) {
        if (count == 1) {
            hex_num = std::stoul(n, 0, 16);
        }
        if (count == 2) {
            count = 0;
            continue;
        }
        count++;
    }


    return 0;

    
}