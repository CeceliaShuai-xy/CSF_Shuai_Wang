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
    uint32_t current_max;
}Cache;

typedef struct {
    uint32_t num_sets;
    uint32_t num_blocks;
    uint32_t num_bytes;
    string allocate;
    string write_command;
    string evictions; 
}Input_param;

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
void checkValidAuguments(Input_param input_param);

//Gigi
Cache initialization_cache(Input_param input_param);

//Cecilla
Stats initialization_stats();

uint32_t log2(uint32_t number);

//Gigi
uint32_t findTag(uint32_t address, uint32_t index_pos);

//Cecilla
uint32_t findIndex(uint32_t address, uint32_t offset_pos, uint32_t index_pos);

bool isHit(uint32_t tag, uint32_t index, Cache* cache);

void updateLRU(uint32_t tag, uint32_t index, Cache *cache);

// initalize map (tag to slot)
void putNewSlot(uint32_t tag, uint32_t index, Input_param* input_param, Cache *cache, Stats* stats);

void load_cache(Cache* cache, uint32_t tag, uint32_t index, Stats* stats, Input_param* input_param);

void save_cache(Cache* cache, uint32_t tag, uint32_t index, Stats* stats, Input_param* input_param);

void mark_dirty(Cache* cache, uint32_t tag, uint32_t index);

bool has_empty(Set* set);

Slot* find_invalid(Set* set);

Slot* eviction_lru(Set set);

Slot* eviction_fifo(Set set);

void printStats(Stats stats);

#endif