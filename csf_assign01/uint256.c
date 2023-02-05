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
  UInt256 *result = malloc(sizeof(UInt256));
  result->data[0] = val;
  for(int i = 1; i < 4; i++) {
    result->data[i] = 0;
  }
  return *result;
}

// Create a UInt256 value from an array of 4 uint64_t values.
// The element at index 0 is the least significant, and the element
// at index 3 is the most significant.
UInt256 uint256_create(const uint64_t data[4]) {
  UInt256 *result = malloc(sizeof(UInt256));
  for(int i = 0; i < 4; i++) {
    result->data[i] = data[i];
  }

  return *result;
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

// Helper function to convert a uint64_t to its two's complement, and sign should indicate positive or negative
char *uint64_to_twos_complement(uint64_t val, int sign) {
  char *binary = malloc(sizeof(char) * 64);
  binary = uint64_to_binary(val);
  
  int count = 0;
  for (int i = 63; i >= 0; i--) {
    char c = val & ((uint64_t)1 << i) ? '0' : '1';
    *(binary + count) = c;
    count++;
  }
  binary = uint64_add(binary, uint64_to_binary((uint64_t) 1),1,NULL,0);
  return binary;
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
  char *hex = (char*)malloc(sizeof(char)* 64);
  char *buff = (char*)malloc(sizeof(char)* (16));
  
  int counter = 0;
  for (int i = 3; i >= 0; i--) {
    uint64_t data = val.data[i];
    sprintf(buff, "%016lx", data);
    for (int j = 0; j<16; j++) {
      hex[counter++] = buff[j];
    }
  }
  int count_digits = 0;
  int i = 0;
  while(hex[i]=='0') {
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
      return_hex[i] = hex[count_digits + i];
    }
  }

  free(buff);
  free(hex);
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
  UInt256 *sum = malloc(sizeof(UInt256));
  int hasCarried = 0;
  for(int i = 0; i < 4; i++) {
    hasCarried = uint64_add(uint64_to_binary(left.data[i]), uint64_to_binary(right.data[i]), hasCarried, sum, i);
  }
  return *sum;
}

// Compute the difference of two UInt256 values.
// Cecelia
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 *result = malloc(sizeof(UInt256));
  int hasCarried = 0;
  for (int i = 0; i < 4; i ++) {
    hasCarried = uint64_add(uint64_to_binary(left.data[i]), uint64_to_twos_complement(right.data[i],0), hasCarried, result, i);
  }
  return *result;
}

// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 product;
  // TODO: implement
  return product;
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
