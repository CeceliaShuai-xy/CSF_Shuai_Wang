// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  // TODO: implement the main function
  unsigned offset = 0;
  // leave room for null terminator
  char buff[18]; 
  buff[17] = '\0';
  // store number of read bytes
  unsigned n = 0; 
  while (1) {
    // num of bytes read
    n = hex_read(buff);
    if (n == 0) {
      break;
    } 

    char hex_offset[8];
    hex_format_offset(offset, hex_offset);
    hex_write_string(hex_offset);

    // string to write
    // 3 spaces + ':' + 15 inbetween spaces + 16*2 = 51
    // 51+1 = 52 add one for null terminator
    char s[52]; 
    for (int i = 0; i < 52; i++) {
      s[i] = ' ';
    }
    s[0] = ':';
    s[51] = '\0';

    // string buff
    char sbuf[2];
  
    for (int i = 2; i < n+2; i++) {
      hex_format_byte_as_hex(buff[i-2],sbuf);
      for (int j = 0; j < 3; j++) {
        if (j != 2) {
          s[3*i+j-4] = hex_to_printable(sbuf[j]);
        } 
      }
    }

    hex_write_string(s);

    for (int i = 0; i<n; i++) {
      buff[i] = hex_to_printable(buff[i]);
    }

    buff[n] = '\n';
    buff[n+1] = '\0';
    hex_write_string(buff);
    offset += n;
  }
}
