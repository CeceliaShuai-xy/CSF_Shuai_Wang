// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations
/* Cecelia Shuai(xshuai3), Gigi Wang(ywang580) */

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);
void textFormatOffset_more(TestObjs *objs);
void textFormatByteasHex_space(TestObjs *objs);
void textFormatByteasHex_letters(TestObjs *objs);
void textFormatByteasHex_symbols(TestObjs *objs);
void textFormatByteasHex_newline(TestObjs *objs);
void testHexToPrintable_unprintable_edge(TestObjs *objs);
void testHexToPrintable_printable_edge(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);
  
  // Add more unit tests
  TEST(textFormatOffset_more);
  TEST(textFormatByteasHex_space);
  TEST(textFormatByteasHex_letters);
  TEST(textFormatByteasHex_symbols);
  TEST(textFormatByteasHex_newline);
  TEST(testHexToPrintable_unprintable_edge);
  TEST(testHexToPrintable_printable_edge);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(0x00000001u, buf);
  ASSERT(0 == strcmp(buf, "00000001"));

  hex_format_offset(0xabcd1234u, buf);
  ASSERT(0 == strcmp(buf, "abcd1234"));
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
}

// test out format offset on zero offset, offset smaller than, 
// equal to, and larger than 16 
void textFormatOffset_more(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(0U, buf);
  ASSERT(0 == strcmp(buf, "00000000"));

  hex_format_offset(16U, buf);
  //printf(buf);
  ASSERT(0 == strcmp(buf, "00000010"));

  hex_format_offset(18U, buf);
  //printf(buf);
  ASSERT(0 == strcmp(buf, "00000012"));

  hex_format_offset(32, buf);
  //printf(buf);
  ASSERT(0 == strcmp(buf, "00000020"));

  hex_format_offset(50, buf);
  //printf(buf);
  ASSERT(0 == strcmp(buf, "00000032"));

  hex_format_offset(64, buf);
  //printf(buf);
  ASSERT(0 == strcmp(buf, "00000040"));
}


 // test on empty space, more letters, and ,!  and other printable
void textFormatByteasHex_space(TestObjs *objs) {
  //Hello, world!\n
  // empty space is at [6]
  char buf[16];
  // [6] should be empty space, hex should be 20
  hex_format_byte_as_hex(objs->test_data_1[6], buf);
  ASSERT(0 == strcmp(buf, "20"));
}


void textFormatByteasHex_letters(TestObjs *objs) {
  char buf[16];
  // more letters 
  hex_format_byte_as_hex(objs->test_data_1[3], buf);
  ASSERT(0 == strcmp(buf, "6c"));

  hex_format_byte_as_hex(objs->test_data_1[4], buf);
  ASSERT(0 == strcmp(buf, "6f"));

  hex_format_byte_as_hex(objs->test_data_1[7], buf);
  ASSERT(0 == strcmp(buf, "77"));
}

void textFormatByteasHex_symbols(TestObjs *objs) {
  // , is at [5]
  // ! is at [12]
  // more like :~!@#$%^&*()
  char buf[16];

  hex_format_byte_as_hex(objs->test_data_1[5], buf);
  ASSERT(0 == strcmp(buf, "2c"));

  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21"));

  hex_format_byte_as_hex(':', buf);
  ASSERT(0 == strcmp(buf, "3a"));
  
  hex_format_byte_as_hex('~', buf);
  ASSERT(0 == strcmp(buf, "7e"));

  hex_format_byte_as_hex('@', buf);
  ASSERT(0 == strcmp(buf, "40"));

  hex_format_byte_as_hex('#', buf);
  ASSERT(0 == strcmp(buf, "23"));

  hex_format_byte_as_hex('$', buf);
  ASSERT(0 == strcmp(buf, "24"));

  hex_format_byte_as_hex('%', buf);
  ASSERT(0 == strcmp(buf, "25"));

  hex_format_byte_as_hex('^', buf);
  ASSERT(0 == strcmp(buf, "5e"));

  hex_format_byte_as_hex('&', buf);
  ASSERT(0 == strcmp(buf, "26"));

  hex_format_byte_as_hex('*', buf);
  ASSERT(0 == strcmp(buf, "2a"));

  hex_format_byte_as_hex('(', buf);
  ASSERT(0 == strcmp(buf, "28"));

  hex_format_byte_as_hex(')', buf);
  ASSERT(0 == strcmp(buf, "29"));

}

void textFormatByteasHex_newline(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  
  hex_format_byte_as_hex('\n', buf);
  ASSERT(0 == strcmp(buf, "0a"));
}

void testHexToPrintable_unprintable_edge(TestObjs *objs) {
  unsigned char val = 31;
  ASSERT('.' == hex_to_printable(val));
  val = 127;
  ASSERT('.' == hex_to_printable(val));
}

void testHexToPrintable_printable_edge(TestObjs *objs) {
  ASSERT(' ' == hex_to_printable(32));
  ASSERT('~' == hex_to_printable(126));
}