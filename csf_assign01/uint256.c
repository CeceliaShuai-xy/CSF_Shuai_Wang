#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // check!
// #include <math.h> // check!
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

// Helper function to convert a uint64_t to array of binary char
char *uint64_to_binary(uint64_t val) {
  char *binary = malloc(sizeof(char) * 64);
  int count = 0;
  for (int i = 63; i >= 0; i--) {
    char c = val & ((uint64_t)1 << i) ? '1' : '0';
    *(binary + count) = c;
    count++;
  }
  return binary;
}

// Helper function to negate uint64_t 
uint64_t reverse_digit_uint64_t(uint64_t val) {
  char *binary = malloc(sizeof(char) * 64);
  int count = 0;
  for (int i = 63; i >= 0; i--) {
    char c = val & ((uint64_t)1 << i) ? '0' : '1';
    *(binary + count) = c;
    count++;
  }
  uint64_t *reversed_val = malloc(sizeof(uint64_t));
  char *endptr = binary + strlen(binary);
  *endptr = '\0';
  *reversed_val = strtoul(binary, NULL, 2);
  free(binary);
  return *reversed_val;
}

// Helper function to create two's complement of a UInt256 value
UInt256 UInt256_to_twos_complement(UInt256 value) {
  for (int i = 0; i < 4; i++) {
    value.data[i] = reverse_digit_uint64_t(value.data[i]);
  }
  UInt256 negation  = uint256_add(uint256_create_from_u64(1U), value);
  return negation;
}

// Create a UInt256 value from a string of hexadecimal digits.
// Gigi
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 result;
  if (strlen(hex) <= 16) {
    result = uint256_create_from_u64(strtoul(hex, NULL, 16));
    return result;
  }
  char copy[strlen(hex)];
  strcpy(copy, hex);
  char *endptr = copy + strlen(copy);
  result = uint256_create_from_u64(strtoul(endptr - 16, NULL, 16));
  endptr -= 16;
  *endptr = '\0';
  for (int i = 1; i < 4; i++) {
    char * holder = endptr - 16;
    if (copy < holder) {
      result.data[i] = strtoul(holder, NULL, 16);
      endptr = holder;
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
// Cecelia
char *uint256_format_as_hex(UInt256 val) {
  char hex_holder[64];
  char *buff = (char*)malloc(sizeof(char)* (16) + 1);
  
  int counter = 0;
  for (int i = 3; i >= 0; i--) {
    uint64_t data = val.data[i];
    sprintf(buff, "%016lx", data);
    for (int j = 0; j<16; j++) {
      hex_holder[counter++] = buff[j];
    }
  }
  int count_digits = 0;
  int i = 0;
  while(hex_holder[i]=='0') {
    count_digits++;
    i++;
  }
  int hex_length = 64 - count_digits;
  char* return_hex;
  if (hex_length <= 0) {
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

// helper function that add two uint64 int and stores it into UInt256 in the index
// return whether this operator has carried over
int uint64_add(char* left_binary, char* right_binary, int hasCarriedOver, UInt256* sum, int index) {
  char* sum_binary = malloc(sizeof(char) * 65);
  //initialized the sum string
  sum_binary[64] = '\0';
  for(int i = 0; i < 64; i++) {
    sum_binary[i] = '0';
  }
  int carry = hasCarriedOver;
  for (int i = 63; i >=0; i--) {
    if (left_binary[i] == '0' && right_binary[i] == '0') {
      if (carry == 1) {
        sum_binary[i] = '1';
        carry = 0;
      }
    } else if (left_binary[i] == '1' && right_binary[i] == '1') {
        if (carry == 0) {
          carry = 1;
        }
        else {
          sum_binary[i] = '1';
        }
    } else {
        if (carry == 0) {
          sum_binary[i] = '1';
        }
      }
  }
  sum->data[index] = strtoul(sum_binary, NULL, 2);
  free(left_binary);
  free(right_binary);
  free(sum_binary);
  return carry;
}

// Compute the sum of two UInt256 values.
// Gigi
UInt256 uint256_add(UInt256 left, UInt256 right) {
  // UInt256 *sum = malloc(sizeof(UInt256));
  // int hasCarried = 0;
  // for(int i = 0; i < 4; i++) {
  //   hasCarried = uint64_add(uint64_to_binary(left.data[i]), uint64_to_binary(right.data[i]), hasCarried, sum, i);
  // }
  // return *sum;

  uint64_t hasCarriedOver = 0;

  UInt256 sum = uint256_create_from_u64(0U);
  for (int i = 0; i < 4; i++) {
    uint64_t individual_left = uint256_get_bits(left, i);
    uint64_t individual_right = uint256_get_bits(right, i);

    uint64_t individual_sum = individual_left + individual_right;
    if ((individual_sum >= individual_left) && (individual_sum >= individual_right)) {
      hasCarriedOver = 0U;
    } else {
      hasCarriedOver = 1U;
    }
    sum.data[i] = individual_sum;
  }
  return sum;
}

// Compute the difference of two UInt256 values.
// Cecelia
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 *result = malloc(sizeof(UInt256));
  *result = uint256_add(left, UInt256_to_twos_complement(right));
  return *result;
}

// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 product;
  // TODO: implement
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
// // for testing purpose
// int main(void) {
//   uint64_t val = 5;
//   char * test = uint64_to_binary(val);
//   printf("\n \n outside:");
//   for(int i = 0; i < 64; i++) {
//     printf("%c", *(test+i));
//   }
//   return 0;
// }
