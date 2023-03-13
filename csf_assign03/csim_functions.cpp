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



// check if input_param is valid
// throw std::invalid_argument() if augments is invalid
void checkValidAuguments(Input_param input_param);

//Gigi
Cache initialization_cache(Input_param input_param) {
    Slot slot = {0, false, 0, 0, false}
    Cache cache;
    Set set;
    for (uint32_t i = 0; i < input_param.num_blocks; i++) {
        set.slots.push_back(slot);
    }
    for (uint32_t i = 0; i < input_param.num_sets; i++) {
        cache.sets.push_back(set);
    }
    cache.current_max = 0;

    return cache;
}

//Cecilla
Stats initialization_stats();

uint32_t log2(uint32_t number);

//Gigi
uint32_t findTag(uint32_t address, uint32_t index_pos) {
    
}

//Cecilla
uint32_t findIndex(uint32_t address, uint32_t offset_pos);

bool isHit(uint32_t tag, uint32_t index, Cache cache);

void updateLRU(uint32_t tag, uint32_t index, Cache *cache);

// initalize map (tag to slot)
void putNewSlot(uint32_t tag, uint32_t index, const Input_param input, Cache *cache, Stats stats);

