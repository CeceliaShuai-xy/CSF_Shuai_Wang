#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#include "csim_functions.h"
#include <sstream>
#include <map>

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
using std::map;  
using std::vector;
using std::find;



// check if input_param is valid
// throw std::invalid_argument() if augments is invalid
void checkValidAuguments(Input_param input_param) {
    return;
}

//Gigi
Cache initialization_cache(Input_param input_param) {
    Slot slot = {0, false, 0, 0, false};
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

//Cecelia
Stats initialization_stats() {
    Stats stats = {0,0,0,0,0,0,0};
    return stats;
}

uint32_t log2(uint32_t number) {
    uint32_t temp = 0;
    while (number != 1){
        number = number >> 1;
        temp ++;
    }
    return temp;
}

//Gigi
uint32_t findTag(uint32_t address, uint32_t index_pos) {
    return ((1<<(32 - index_pos)) - 1) & (address >> index_pos);
}

//Cecelia
uint32_t findIndex(uint32_t address, uint32_t offset_pos, uint32_t index_pos) {
    return (((1 << (index_pos - offset_pos)) - 1) & (address >> offset_pos));
}

// change cache to pointers ? 
bool isHit(uint32_t tag, uint32_t index, Cache* cache) {
    Set set = cache->sets.at(index);
    std::map<uint32_t, Slot *>::iterator it = set.tagToSlot.find(tag);
    return !(it == set.tagToSlot.end());
}

void load_cache(Cache* cache, uint32_t tag, uint32_t index, Stats* stats, Input_param* input_param) {
    // check hit
    Input_param input = *input_param;
    stats->total_loads++;
    if (isHit(tag, index, cache)) {
        stats->load_hits++;
        stats->total_cycles++;
        if (input.evictions == "lru") {
            updateLRU(tag, index, cache);
        } 
    } else {
        stats->total_cycles += (100 * (input.num_bytes) / 4);
        stats->load_misses++;
        putNewSlot(tag, index, input_param, cache, stats);
    }
}


void save_cache(Cache* cache, uint32_t tag, uint32_t index, Stats* stats, Input_param* input_param){
    // check hit
    Input_param input = *input_param;
    stats->total_stores++;
    if (isHit(tag, index, cache)) {
        stats->store_hits++;
        if (input.evictions == "lru") {
            updateLRU(tag, index, cache);
        } 
        if (input.write_command == "write-through") {
            stats->total_cycles += 100 + 1;
        } else {
            mark_dirty(cache, tag, index);
            stats->total_cycles++;
        }
    } else {
        stats->store_misses++;
        if (input.allocate == "write-allocate") {
            stats->total_cycles += (100 * (input.num_bytes) / 4) + 1;
            putNewSlot(tag, index, input_param, cache, stats);
        } else {
            stats->total_cycles += 100;
        }
    }
}

void mark_dirty(Cache* cache, uint32_t tag, uint32_t index) {
    std::map<uint32_t, Slot *> map = cache->sets.at(index).tagToSlot;
    map.at(tag)->dirty = true;
}

void updateLRU(uint32_t tag, uint32_t index, Cache *cache) {
    std::map<uint32_t, Slot *> map = cache->sets.at(index).tagToSlot;
    map.at(tag)->access_ts = cache->current_max;
}

// initalize map (tag to slot)
void putNewSlot(uint32_t tag, uint32_t index, Input_param* input_param, Cache *cache, Stats* stats) {
    // check if there's invalid slots
    Input_param input = *input_param;
    Set* set = &cache->sets.at(index);
    std::map<uint32_t, Slot *>::iterator it;
    Slot* original_slot;

    if (has_empty(set)) {
        // find invalid spot 
        original_slot = find_invalid(set);
        *original_slot = {tag, true, cache->current_max, cache->current_max, false};
        set->tagToSlot.insert({tag,original_slot});
    } else {
        if (input.evictions == "lru") {
            // kick out slot with the least access_ts
            original_slot = eviction_lru(*set);
        } else {
            // fifo
            original_slot = eviction_fifo(*set);
        }

        if (original_slot->dirty) {
            stats->total_cycles+=(100 * (input.num_bytes) / 4);
        }
        set->tagToSlot.erase(original_slot->tag);
        *original_slot = {tag, true, cache->current_max, cache->current_max,false};
        set->tagToSlot.insert({tag,original_slot});
    }
}

bool has_empty(Set* set) {
    return set->tagToSlot.size() < set->slots.size();
}

Slot* find_invalid(Set* set) {
    for (vector<Slot>::iterator it = (set->slots).begin(); it != set->slots.end(); ++it) {
        if (it->valid == false) {
            return &(*it);
        }
    }
    return NULL;
}

Slot* eviction_lru(Set set) {
    uint32_t min_ts = UINT32_MAX;
    std::map<uint32_t, Slot *>::iterator temp_it;
    for (std::map<uint32_t, Slot *>::iterator it = set.tagToSlot.begin(); it != set.tagToSlot.end(); ++it) {
        if (it->second->access_ts < min_ts) {
            min_ts = it->second->access_ts;
            temp_it = it;
        }
    }
    return &(*temp_it->second);
}

Slot* eviction_fifo(Set set) {
    uint32_t min_ts = UINT32_MAX;
    std::map<uint32_t, Slot *>::iterator temp_it;
    for (std::map<uint32_t, Slot *>::iterator it = set.tagToSlot.begin(); it != set.tagToSlot.end(); ++it) {
        if (it->second->load_ts < min_ts) {
            min_ts = it->second->load_ts;
            temp_it = it;
        }
    }
    return &(*temp_it->second);
}

void printStats(Stats stats) {
    cout << "Total loads: " << stats.total_loads << endl;
    cout << "Total stores: " << stats.total_stores << endl;
    cout << "Load hits: " << stats.load_hits << endl;
    cout << "Load misses: " << stats.load_misses << endl;
    cout << "Store hits: " << stats.store_hits << endl;
    cout << "Store misses: " << stats.store_misses << endl;
    cout << "Total cycles: " << stats.total_cycles << endl;
}



