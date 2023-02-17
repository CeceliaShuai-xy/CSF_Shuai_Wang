// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  // TODO: implement the main function
  unsigned offset = 0;
  
  // store number of read bytes
  unsigned n = 0; 
  const char *fmtstr = ":";
  const char *empty = " ";


  while (1) {
    // leave room for null terminator //and newline
    char buff[18]; 
    // num of bytes read
    n = hex_read(buff);
    if (n == 0) {
      break;
    } 

    char hex_offset[9];

    hex_format_offset(offset, hex_offset);
    hex_write_string(hex_offset);
    hex_write_string(fmtstr);

    // string buff
    char sbuf[3];
    for (int i = 0; i<n; i++) {
      hex_format_byte_as_hex(buff[i],sbuf);
      buff[i] = hex_to_printable(buff[i]);
      
      hex_write_string(empty);
      hex_write_string(sbuf);
    }
    // hex_write_string(s);
    for (int j = n; j < 16; j++) {
      hex_write_string(empty);
      hex_write_string(empty);
      hex_write_string(empty);
    }

    hex_write_string(empty);
    hex_write_string(empty);

    buff[n] = '\n';
    buff[n+1] = '\0';
    hex_write_string(buff);
    offset += n;
  }
}
