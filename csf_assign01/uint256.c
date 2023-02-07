#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "uint256.h"

// Create a UInt256 value from a single uint64_t value.
// Only the least-significant 64 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u64(uint64_t val) {
  UInt256 result;
  result.data[0] = val;
  for(int i = 1; i < 4; i++) {
    result.data[i] = 0;
  }
  return result;
}

// Create a UInt256 value from an array of 4 uint64_t values.
// The element at index 0 is the least significant, and the element
// at index 3 is the most significant.
UInt256 uint256_create(const uint64_t data[4]) {
  UInt256 result;
  for(int i = 0; i < 4; i++) {
    result.data[i] = data[i];
  }
  return result;
}

// Helper function to create two's complement of a UInt256 value
UInt256 UInt256_to_twos_complement(UInt256 value) {
  for (int i = 0; i < 4; i++) {
    value.data[i] = ~(value.data[i]);
  }
  UInt256 negation  = uint256_add(uint256_create_from_u64(1U), value);
  return negation;
}

// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 result;
  // hex map to Uint256[0] if hex is less than 16 char
  if (strlen(hex) <= 16) {
    return uint256_create_from_u64(strtoul(hex, NULL, 16));
  }
  // make a copy of hex for modification
  char copy[strlen(hex)];
  strcpy(copy, hex);
  char *endptr = copy + strlen(copy);
  result = uint256_create_from_u64(strtoul(endptr - 16, NULL, 16));
  endptr -= 16;
  *endptr = '\0';
  // break the copied hex in chunk of 16 char or less using '\0' to use strtoul
  for (int i = 1; i < 4; i++) {
    char * holder_ptr = endptr - 16;
    if (copy < holder_ptr) {
      result.data[i] = strtoul(holder_ptr, NULL, 16);
      endptr = holder_ptr;
      *endptr = '\0';
    } else {
      result.data[i] = strtoul(copy, NULL, 16);
      break;
    }
  }
  return result;
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  char hex_holder[64];
  char *buff = (char*)malloc(sizeof(char)* (16) + 1);
  
  // convert UInt256 into hex and store each char in hex_holder array
  int counter = 0;
  for (int i = 3; i >= 0; i--) {
    uint64_t data = val.data[i];
    sprintf(buff, "%016lx", data);
    for (int j = 0; j<16; j++) {
      hex_holder[counter++] = buff[j];
    }
  }
  // count the number of leading zeros
  int count_digits = 0;
  int i = 0;
  while(hex_holder[i]=='0') {
    count_digits++;
    i++;
  }
  // remove leading zeros by copying hex_holder to a new array return_hex
  int hex_length = 64 - count_digits;
  char* return_hex;
  if (hex_length <= 0) { // edge case where val is 0
    return_hex = malloc(sizeof(char) * 2);
    return_hex[0] = '0';
    return_hex[1] = '\0';
  } else {
    return_hex = malloc(hex_length + 1);
    return_hex[hex_length] = '\0';
    for (int i = 0; i < hex_length; i++) {
      return_hex[i] = hex_holder[count_digits + i];
    }
  }

  free(buff);
  return return_hex;
}


// Get 64 bits of data from a UInt256 value.
// Index 0 is the least significant 64 bits, index 3 is the most
// significant 64 bits.
uint64_t uint256_get_bits(UInt256 val, unsigned index) {
  uint64_t bits = val.data[index];
  return bits;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {

  uint64_t hasCarriedOver = 0;

  UInt256 sum = uint256_create_from_u64(0U);
  for (int i = 0; i < 4; i++) {
    // for each elements in UInt256, find its sum considering overflow from previous elemesnt
    uint64_t individual_left = uint256_get_bits(left, i);
    uint64_t individual_right = uint256_get_bits(right, i);
    uint64_t individual_sum = individual_left + individual_right + hasCarriedOver;

    // update overflow
    if (individual_sum < individual_left || individual_sum < individual_right) {
      hasCarriedOver = 1U;
    } else {
      hasCarriedOver = 0U;
    }
    sum.data[i] = individual_sum;
  }
  return sum;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 result = uint256_add(left, UInt256_to_twos_complement(right));
  return result;
}

// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 product = uint256_create_from_u64(0UL);
  for (int i = 0; i < 256; i++) {
    if (uint256_bit_is_set(right,i) == 1) {
      product = uint256_add(product, uint256_leftshift(left,i));
    }
  }
  // if no 1 bit in right (times 0), return 0
  return product;
}

// Check whether a particular bit at index is set to 1 in a UInt256 data, return true(1) or false(0)
int uint256_bit_is_set(UInt256 val, unsigned index) {
  int array_index;
  if (index >= 192) {
    array_index = 3;
  } 
  else if (index >= 128) {
    array_index = 2;
  } 
  else if (index >= 64) {
    array_index = 1;
  } 
  else {
    array_index = 0;
  }

  uint64_t value = val.data[array_index];
  if ((value >> (index - 64*array_index)) & (uint64_t)1) {
    return 1;
  } 
  return 0;
}

//Helper function for within 64-bits shift
//Pre: 0 < shift <=64
UInt256 uint256_leftshift_piecewise(UInt256 val, unsigned shift) {
  if (shift == 0) {
    return val;
  }
  for (int i = 3; i >= 1; i--) {
    uint64_t value = val.data[i];
    uint64_t old_bits = value<<shift;
    //bits that come from the next data[i-1]
    uint64_t new_bits = val.data[i-1]>>((unsigned int)64-shift);
    val.data[i] = old_bits + new_bits;
  }
  //handle the data[0] case
  val.data[0] = val.data[0]<<shift;
  return val;
}

// Left shit UInt 256 value by a specified number
// pre: shift >= 0 
UInt256 uint256_leftshift(UInt256 val, unsigned shift) {
  if (shift == 0) {
    return val;
  }

  if (shift < 64) {
    return uint256_leftshift_piecewise(val, shift);
  } else {
    UInt256 new_val = uint256_create_from_u64(0UL);
    int times = shift/64;
    if (times >= 4) {
      //shifts take all value out of frame
      return new_val;
    } else {
      int counter = 0;
      int modular = shift%64;
      //first shift by multiples of 64
      for (int i = times; i < 4; i++) {
        new_val.data[i] = val.data[counter++];
      }
      //shift the reminding number
      return uint256_leftshift_piecewise(new_val, modular);
    }
  }
  return val;
}