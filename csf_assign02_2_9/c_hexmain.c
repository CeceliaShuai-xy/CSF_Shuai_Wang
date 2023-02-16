// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  // TODO: implement the main function
  unsigned offset = 0;
  // leave room for null terminator
  char buff[18]; 
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
    char s[50]; 
    for (int i = 0; i < 53; i++) {
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
          hex_to_printable(sbuf[j]);
          //s[(i-1)*2 + j] = sbuf[j];
          s[3*i+j-4] = sbuf[j];
        } 
      }
    }

    hex_write_string(s);

    if (n<16) {
      buff[n-1] = '.';
      buff[n] = '\n';
      hex_write_string(buff);
    } else {
    buff[16] = '\n';
    hex_write_string(buff);
    }
    offset += n;
  }
}
