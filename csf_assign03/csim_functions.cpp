/*
 * Cpp helper functions to implement cache
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

/*
 * Check if input parameters are valid in terms of the number
 * and strings. Throw std::invalid_argument() if augments
 * are invalid.
 * 
 * Parameters:
 *      input_param - a struct that stores all input arguments
 * 
 * Returns:
 *      void
 *  
 */
void checkValidAuguments(Input_param* input_param) {
    check_valid_num(input_param);
    check_valid_string(input_param);
}

/*
 * Check if the numerical input are valid. Throw std::invalid_argument()
 * if augments are invalid.
 * 
 * Parameters:
 *      input_param - a struct that stores all input arguments
 * 
 * Returns:
 *      void
 *  
 */
void check_valid_num(Input_param* input_param) {
    if (!(is_power_of_2(input_param->num_sets) && is_power_of_2(input_param->num_blocks) && is_power_of_2(input_param->num_bytes))) {
        throw std::invalid_argument("Error: numbers are not power of 2.");
    }
    if (input_param->num_bytes < 4) {
        throw std::invalid_argument("Error: number of bytes is less than 4.");
    }
    if (log2(input_param->num_sets) + log2(input_param->num_bytes) >= 32) {
        throw std::invalid_argument("Error: cannot interpret 32-bit memory addresses.");
    }
}

/*
 * Helper function to check if input n is a power of 2.
 * 
 * Parameters:
 *      n - the input number, number of sets, number of blocks 
 * and number of bytes in this particular case
 * 
 * Returns:
 *      true - if n is a power of 2
 *      false - otherwise
 *  
 */
bool is_power_of_2(uint32_t n) {
    return (n > 0) && (n & (n - 1)) == 0;
}

/*
 * Check if string inputs are valid. Throw std::invalid_argument()
 * when they are not.
 * 
 * Parameters:
 *      input_param - a struct that stores all input arguments
 * 
 * Returns:
 *      void
 *  
 */
void check_valid_string(Input_param* input_param) {
    if ((input_param->allocate != "write-allocate") && (input_param->allocate != "no-write-allocate")) {
        throw std::invalid_argument("Error: invalid allocation method");
    }
    if ((input_param->write_command != "write-through") && (input_param->write_command != "write-back")) {
        throw std::invalid_argument("Error: invalid writing method.");
    }
    if ((input_param->evictions != "lru") && (input_param->evictions != "fifo")) {
        throw std::invalid_argument("Error: invalid evictions.");
    }
    if ((input_param->write_command == "write-back") && (input_param->allocate == "no-write-allocate")) {
        throw std::invalid_argument("Error: write-back and no-write-allocate cannot be together");
    }
}

/*
 * Initialize the cache with input parameters. 
 * 
 * 
 * Parameters:
 *      input_param - a struct that stores all input arguments
 * 
 * Returns:
 *      cache - the initialized cache
 *  
 */
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

/*
 * Initialize the cache stats to be all zeros.
 * 
 * 
 * Parameters:
 *      N/A
 * 
 * Returns:
 *      stats - a struct that stores all the statistics
 *  
 */
Stats initialization_stats() {
    Stats stats = {0,0,0,0,0,0,0};
    return stats;
}

/*
 * Helper function to calculate the log2 of an integer.
 * 
 * 
 * Parameters:
 *      number - a uint32_t type number
 * 
 * Returns:
 *      temp - the log2(number) result
 *  
 */
uint32_t log2(uint32_t number) {
    uint32_t temp = 0;
    while (number != 1){
        number = number >> 1;
        temp ++;
    }
    return temp;
}

/*
 * Calculate the tag.
 * 
 * 
 * Parameters:
 *     address - the memory address
 *     index_pos - index position
 * 
 * Returns:
 *      address >> index_pos (the calculated tag)
 *  
 */
uint32_t findTag(uint32_t address, uint32_t index_pos) {
    return address >> index_pos;
}

/*
 * Find the index using addressm offset_position and index position.
 * 
 * 
 * Parameters:
 *     address - the memory address
 *     offset_pos - index of the 'offset'
 *     index_pos - index of the 'index'
 * 
 * Returns:
 *      the calculated index
 *  
 */
uint32_t findIndex(uint32_t address, uint32_t offset_pos, uint32_t index_pos) {
    return (((1 << (index_pos - offset_pos)) - 1) & (address >> offset_pos));
}

/*
 * Check if tag can be found in the cache at certain index.
 * 
 * 
 * Parameters:
 *     tag - the tag
 *     index - the index
 *     cache - pointer to the cache
 * 
 * Returns:
 *      true - when cache has found the slot with specified tag and index
 *      false - otherwise
 *  
 */
bool isHit(uint32_t tag, uint32_t index, Cache* cache) {
    Set set = cache->sets.at(index);
    std::map<uint32_t, Slot *>::iterator it = set.tagToSlot.find(tag);
    return !(it == set.tagToSlot.end());
}

/*
 * Load cache. Check hit first. Update the cache slot if there is 
 * a miss (not hit). Update the cache statistics and access 
 * timestamps accordingly.
 * 
 * 
 * Parameters:
 *     cache - poiner to the cache
 *     tag - the tag
 *     index - the index
 *     stats - pointer to the struct that stores the cache statistics 
 *     input-param - pointer to the struct that stores the input parameters
 *     
 * 
 * Returns:
 *      void
 *  
 */
void load_cache(Cache* cache, uint32_t tag, uint32_t index, Stats* stats, Input_param* input_param) {
    // check hit
    Input_param input = *input_param;
    stats->total_loads++;
    Set *set = &(cache->sets.at(index));
    std::map<uint32_t, Slot *>::iterator it = set->tagToSlot.find(tag);
    if (it != set->tagToSlot.end()) {
        stats->load_hits++;
        stats->total_cycles++;
        if (input.evictions == "lru") {
            it->second->access_ts = cache->current_max;
        } 
    } else {
        stats->total_cycles += (100 * (input.num_bytes) / 4);
        stats->load_misses++;
        putNewSlot(tag, index, input_param, cache, stats);
    }
}

/*
 * Save cache. Check hit first. If there is a miss, and the input
 * parameter specifies wrtie-through, update the cache slot.
 * And update the cache statistics and access timestamps accordingly.
 * 
 * 
 * Parameters:
 *     cache - pointer to the cache
 *     tag - the tag
 *     index - the index
 *     stats - pointer to the struct that stores the cache statistics 
 *     input-param - pointer to the struct that stores the input parameters
 *     
 * 
 * Returns:
 *      void
 *  
 */
void save_cache(Cache* cache, uint32_t tag, uint32_t index, Stats* stats, Input_param* input_param){
    // check hit
    Input_param input = *input_param;
    stats->total_stores++;
    Set* set = &(cache->sets.at(index));
    std::map<uint32_t, Slot *>::iterator it = set->tagToSlot.find(tag);
    if (it != set->tagToSlot.end()) {
        stats->store_hits++;
        if (input.evictions == "lru") {
            it->second->access_ts = cache->current_max;
        } 
        if (input.write_command == "write-through") {
            stats->total_cycles += 100 + 1;
        } else {
            it->second->dirty = true;
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

/*
 * Update the cache slot. Direclty insert the slot if there is invalid
 * slot, else, erase one slot accoridng to the eviction rule and insert 
 * the new slot with the tag. 
 * 
 * 
 * Parameters:
 *     tag - the tag
 *     index - the index
 *     input-param - pointer to the struct that stores the input parameters
*      cache - pointer to the cache
 *     stats - pointer to the struct that stores the cache statistics 
 *     
 * 
 * Returns:
 *      void
 *  
 */
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
            original_slot = eviction_lru(set);
        } else {
            // fifo
            original_slot = eviction_fifo(set);
        }

        if (original_slot->dirty) {
            stats->total_cycles+=(100 * (input.num_bytes) / 4);
        }
        set->tagToSlot.erase(original_slot->tag);
        *original_slot = {tag, true, cache->current_max, cache->current_max,false};
        set->tagToSlot.insert({tag,original_slot});
    }
}

/*
 * Helper function to check is the set is empty.
 * 
 * Parameters:
 *     set - pointer to a struct that simulates the set
 * 
 * Returns:
 *      true - if used set size is smaller than the capacity
 *      false - otherwise
 *  
 */
bool has_empty(Set* set) {
    return set->tagToSlot.size() < set->slots.size();
}

/*
 * Helper function to check if there is invalid slot.
 * 
 * Parameters:
 *     set - pointer to a struct that simulates the set
 * 
 * Returns:
 *      address of the invalid slot
 *      Null - if there is no invalid slot
 *  
 */
Slot* find_invalid(Set* set) {
    for (vector<Slot>::iterator it = (set->slots).begin(); it != set->slots.end(); ++it) {
        if (it->valid == false) {
            return &(*it);
        }
    }
    return NULL;
}

/*
 * Evict the slot according to the LRU rule by finding the 
 * slot with the smallest access timestamps.
 * 
 * Parameters:
 *     set - pointer to a struct that simulates the set
 * 
 * Returns:
 *      pointer to the slot with smallest access timestamps
 *  
 */
Slot* eviction_lru(Set* set) {
    uint32_t min_ts = UINT32_MAX;
    std::map<uint32_t, Slot *>::iterator temp_it;
    for (std::map<uint32_t, Slot *>::iterator it = set->tagToSlot.begin(); it != set->tagToSlot.end(); ++it) {
        if (it->second->access_ts < min_ts) {
            min_ts = it->second->access_ts;
            temp_it = it;
        }
    }
    return &(*temp_it->second);
}

/*
 * Evict the slot according to the FIFO rule by finding the 
 * slot with the smallest load timestamps.
 * 
 * Parameters:
 *     set - pointer to a struct that simulates the set
 * 
 * Returns:
 *      pointer to the slot with smallest load timestamps
 *  
 */
Slot* eviction_fifo(Set* set) {
    uint32_t min_ts = UINT32_MAX;
    std::map<uint32_t, Slot *>::iterator temp_it;
    for (std::map<uint32_t, Slot *>::iterator it = set->tagToSlot.begin(); it != set->tagToSlot.end(); ++it) {
        if (it->second->load_ts < min_ts) {
            min_ts = it->second->load_ts;
            temp_it = it;
        }
    }
    return &(*temp_it->second);
}

/*
 * Helper functions to print the cache stats.
 * 
 * Parameters:
 *     stats - the struct that stores all the cache statistics
 * 
 * Returns:
 *      void
 *  
 */
void printStats(Stats stats) {
    cout << "Total loads: " << stats.total_loads << endl;
    cout << "Total stores: " << stats.total_stores << endl;
    cout << "Load hits: " << stats.load_hits << endl;
    cout << "Load misses: " << stats.load_misses << endl;
    cout << "Store hits: " << stats.store_hits << endl;
    cout << "Store misses: " << stats.store_misses << endl;
    cout << "Total cycles: " << stats.total_cycles << endl;
}



