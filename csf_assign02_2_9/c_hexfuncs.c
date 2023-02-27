// C implementation of hexdump functions
/* Cecelia Shuai(xshuai3), Gigi Wang(ywang580) */
#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO: add function implementations here

unsigned hex_read(char data_buf[]) {
    return read(STDIN_FILENO, data_buf, 16);
}

void hex_write_string(const char s[]) {
    int count = 0;
    while (s[count] != '\0') {
        count++;
    }
    write(1, s, count);
}

void hex_format_offset(unsigned offset, char sbuf[]) {
    // first fill the buffer with 0s
    for (int i = 0; i < 8; i++) {
        sbuf[i] = '0';
    }
    unsigned temp_offset = offset; //use edi for temp_offset
    unsigned temp;  // use r11d for temp
    int i = 7;
    // format into hex
    while (temp_offset != 0) {
        temp = temp_offset & 15;
        if (temp < 10) {
            temp += 48;
        } else {
            temp += 87;
        }
        // update buffer with hex
        sbuf[i--] = temp;
        temp_offset = temp_offset >> 4;
    }

    // add null terminator
    sbuf[8] = '\0';
}

void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
    unsigned char temp_byteval = byteval;
    unsigned char second = temp_byteval & 15;
    unsigned char first = temp_byteval >> 4;
    if (second < 10) {
        sbuf[1] = second + 48;
    } else {
        sbuf[1] = second + 87;
    }

    if (first < 10) {
        sbuf[0] = first + 48;
    } else {
        sbuf[0] = first + 87;
    }
    sbuf[2] = '\0';
}

char hex_to_printable(unsigned char byteval) {
    if (byteval >= 32 && byteval < 127) {
        return byteval;
    }
    return (char)46;
}