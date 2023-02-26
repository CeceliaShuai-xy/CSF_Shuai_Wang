// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  // TODO: implement the main function
  unsigned offset = 0;
  
  // store number of read bytes
  unsigned n = 0; 
  // strings to keep the format
  const char *fmtstr = ":";
  const char *empty = " ";


  while (1) {
    // leave room for null terminator //and newline 16+1+1 = 18
    char buff[18]; 

    // num of bytes read
    n = hex_read(buff);
    // break out of while loop if nothing to read
    if (n == 0) {
      break;
    } 

    // 8 char + null terminator
    char hex_offset[9];

    // update hex_offset string and write it with format string ': '
    hex_format_offset(offset, hex_offset);
    hex_write_string(hex_offset);
    hex_write_string(fmtstr);

    // string buffer to format each byte as hex, leave room for null terminator
    char sbuf[3];
    for (int i = 0; i<n; i++) {
      hex_format_byte_as_hex(buff[i],sbuf);
      buff[i] = hex_to_printable(buff[i]);
      
      // format be like ' 0a'
      hex_write_string(empty);
      hex_write_string(sbuf);
    }

    for (int j = n; j < 16; j++) {
      // if read in string is smaller than 16, padd 3 empty spaces for each byte
      hex_write_string(empty);
      hex_write_string(empty);
      hex_write_string(empty);
    }
    // add two empty spaces before print buff
    hex_write_string(empty);
    hex_write_string(empty);

    // print buff
    buff[n] = '\n';
    buff[n+1] = '\0';
    hex_write_string(buff);

    // update offset
    offset += n;
  }
  return 0;
}
