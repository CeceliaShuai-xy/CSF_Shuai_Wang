#ifndef CSIM_FUNCTIONS_H
#define CSIM_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <map>


using std::string;
using std::vector;
using std::map;



typedef struct {
    uint32_t tag;
    bool valid;
    uint32_t load_ts;
    uint32_t access_ts;
    bool dirty;
}Slot;

typedef struct {
    vector<Slot> slots;
    map<uint32_t, Slot *> tagToSlot;
}Set;

typedef struct {
    vector<Set> sets;
}Cache;

typedef struct {
    uint32_t num_sets;
    uint32_t num_blocks;
    uint32_t num_bytes;
    string allocate;
    string write_command;
    string evictions; 
}Input_parm;

typedef struct {
    uint32_t total_loads;
    uint32_t total_stores;
    uint32_t load_hits;
    uint32_t load_misses;
    uint32_t store_hits;
    uint32_t store_misses;
    uint32_t total_cycles;
}Stats;

// check if input_param is valid
// throw std::invalid_argument() if augments is invalid
void checkValidAuguments(Input_parm input_parm);

Cache initialization_cache(Input_parm input_parm);

Stats initialization_stats();

uint32_t log2(uint32_t number);

uint32_t findTag(unit32_t address, uint32_t index_pos);

uint32_t findIndex(uint32_t address, uint32_t offset_pos);


#endif