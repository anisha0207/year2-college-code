#include <cmath>
#include <iostream>
#include <map> //key-value pair 
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept> //std::runtime_error //std::logic_error
#include <vector>// sample -- vec = std::vector<u8>
#include <type_traits> //std::enable_if //std::is_same -- type specific handling
#include "pack109.hpp"

using std::string;
using std::vector;
using vec = std::vector<u8>;

// Numeric types version
template <class T>
typename std::enable_if<!std::is_same<T, string>::value, int>::type
test(const char *label, T lhs, T rhs) {
  printf("%s: ", label);
  if (lhs == rhs) {
    printf("Passed\n");
    return 1;
  } else {
    printf("Failed\n");
    printf("  lhs=%x\n", static_cast<unsigned int>(lhs));//expected
    printf("  rhs=%x\n", static_cast<unsigned int>(rhs));//what is actually being passed based off of test case
    exit(1);
  }
}

// String version
int test(const char *label, string lhs, string rhs) {
  printf("%s: ", label);
  if (lhs == rhs) {
    printf("Passed\n");
    return 1;
  } else {
    printf("Failed\n");
    std::cout << "  lhs=" << lhs << "\n";
    std::cout << "  rhs=" << rhs << "\n";
    exit(1);
  }
}

// Numeric vector version
template <typename T>
typename std::enable_if<!std::is_same<T, string>::value>::type
testvec(const char *name, std::vector<T> actual, std::vector<T> expected) {
  printf("%s: ", name);
  if (actual == expected) {
    printf("Pass\n");
  } else {
    printf("Fail\n");
    printf("  Expected: ");
    for (const auto &item : expected)
      std::cout << static_cast<int>(item) << " ";
    printf("\n  Actual:   ");
    for (const auto &item : actual)
      std::cout << static_cast<int>(item) << " ";
    printf("\n");
    exit(1);
  }
}

// String vector version
void testvec(const char *name, std::vector<string> actual, std::vector<string> expected) {
  printf("%s: ", name);
  if (actual == expected) {
    printf("Pass\n");
  } else {
    printf("Fail\n");
    printf("  Expected: ");
    for (const auto &item : expected)
      std::cout << item << " ";
    printf("\n  Actual:   ");
    for (const auto &item : actual)
      std::cout << item << " ";
    printf("\n");
    exit(1);
  }
}

// Original vec implementation
int testvec(const char *label, vec lhs, vec rhs) {
  printf("%s: ", label);
  if (lhs == rhs) {
    printf("Passed\n");
    return 1;
  } else {
    printf("Failed\n");
    printf("  lhs=");//expected
    pack109::printVec(lhs);
    printf("\n");
    printf("  rhs=");//what it actually print
    pack109::printVec(rhs);
    printf("\n");
    exit(1);
  }
}
// Float comparison helper
bool float_equal(float a, float b) { 
  return fabs(a - b) < 0.0001f; 
}

// Double comparison helper
bool double_equal(double a, double b) { 
  return fabs(a - b) < 0.0000001; 
}

// Test functions
void test_float(const char *name, float actual, float expected) {
  printf("%s: ", name);
  if (float_equal(actual, expected)) {
    printf("Passed\n");
  } else {
    printf("Failed\n");
    printf("  Actual:   %a\n", actual);
    printf("  Expected: %a\n", expected);
    printf("  Difference: %.8f\n", fabs(actual - expected));
    exit(1);
  }
}

void test_double(const char *name, double actual, double expected) {
  printf("%s: ", name);
  if (double_equal(actual, expected)) {
    printf("Pass\n");
  } else {
    printf("Fail (Expected %f, Got %f)\n", expected, actual);
    exit(1);
  }
}

int main() {
  // Test bool
  vec v1{0xa0};
  vec bytes1 = pack109::serialize(true);
  testvec("Test 1 - true ser", bytes1, v1);

  bool bool_true = pack109::deserialize_bool(bytes1);
  test("Test 2 - true de", bool_true, true);

  vec v2{0xa1};
  vec bytes2 = pack109::serialize(false);
  testvec("Test 3 - false ser", bytes2, v2);

  bool bool_false = pack109::deserialize_bool(bytes2);
  test("Test 4 - false de", bool_false, false);

  // Test Integers
  u8 item1 = 0x42;
  vec v3{0xa2, 0x42};
  vec bytes3 = pack109::serialize(item1);
  testvec("Test 5 - u8 ser", bytes3, v3);

  u8 int_u8 = pack109::deserialize_u8(bytes3);
  test("Test 6 - u8 de", int_u8, item1);

  u32 item_u32 = 0xDEADBEEF;
  vec v4{0xa3, 0xDE, 0xAD, 0xBE, 0xEF};
  vec bytes4 = pack109::serialize(item_u32);
  testvec("Test 7 - u32 ser", bytes4, v4);

  u32 int_u32 = pack109::deserialize_u32(bytes4);
  test("Test 8 - u32 de", int_u32, item_u32);

  u64 item_u64 = 0xCAFEBABEDEADBEEF;
  vec v5{0xa4, 0xCA, 0xFE, 0xBA, 0xBE, 0xDE, 0xAD, 0xBE, 0xEF};
  vec bytes5 = pack109::serialize(item_u64);
  testvec("Test 9 - u64 ser", bytes5, v5);

  u64 int_u64 = pack109::deserialize_u64(bytes5);
  test("Test 10 - u64 de", int_u64, item_u64);

  i8 item_i8 = -100;
  vec v6{0xa5, 0x9C};
  vec bytes6 = pack109::serialize(item_i8);
  testvec("Test 11 - i8 ser", bytes6, v6);

  i8 int_i8 = pack109::deserialize_i8(bytes6);
  test("Test 12 - i8 de", int_i8, item_i8);

  i32 item_i32 = -2147483647;
  vec v7{0xa6, 0x80, 0x00, 0x00, 0x01};
  vec bytes7 = pack109::serialize(item_i32);
  testvec("Test 13 - i32 ser", bytes7, v7);

  i32 int_i32 = pack109::deserialize_i32(bytes7);
  test("Test 14 - i32 de", int_i32, item_i32);

  i64 item_i64 = -9223372036854775807;
  vec v8{0xa7, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
  vec bytes8 = pack109::serialize(item_i64);
  testvec("Test 15 - i64 ser", bytes8, v8);

  i64 int_i64 = pack109::deserialize_i64(bytes8);
  test("Test 16 - i64 de", int_i64, item_i64);

  f32 item_f32 = 3.14159265358979f;
  vec v9{0xa8, 0x40, 0x49, 0x0f, 0xdb};
  vec bytes9 = pack109::serialize(item_f32);
  testvec("Test 17 - f32 ser", bytes9, v9);

  test_float("Test 18 - f32 de", pack109::deserialize_f32(bytes9), item_f32);

  f64 item_f64 = 2.718281828459045;
  vec v10{0xa9, 0x40, 0x05, 0xbf, 0x0a, 0x8b, 0x14, 0x57, 0x69};
  vec bytes10 = pack109::serialize(item_f64);
  testvec("Test 19 - f64 ser", bytes10, v10);

  test_double("Test 20 - f64 de", pack109::deserialize_f64(bytes10), item_f64);

  // test string
  string test_str = "test";
  vec v13{0xaa, 0x04, 't', 'e', 's', 't'};
  vec bytes13 = pack109::serialize(test_str);
  testvec("Test 21 - string ser", bytes13, v13);

  string deserialized_str = pack109::deserialize_string(bytes13);
  test("Test 22 - string de", deserialized_str, test_str);

  // Test u8 array
  std::vector<u8> u8_array = {0x01, 0x02, 0x03};
  vec v17{0xac, 0x03, 0x01, 0x02, 0x03};
  vec bytes17 = pack109::serialize(u8_array);
  testvec("Test 23 - u8 array ser", bytes17, v17);

  std::vector<u8> deserialized_u8 = pack109::deserialize_vec_u8(bytes17);
  testvec("Test 24 - u8 array de", deserialized_u8, u8_array);

  // Test u64 array
  std::vector<u64> u64_array = {0xABCDEF, 0x123456};
  vec v18{0xac, 0x02, 0xa4, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAB, 0xCD, 0xEF, 0xa4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56};
  vec bytes18 = pack109::serialize(u64_array);
  testvec("Test 25 - u64 array ser", bytes18, v18);

  std::vector<u64> deserialized_u64 = pack109::deserialize_vec_u64(bytes18);
  testvec<u64>("Test 26 - u64 array de", deserialized_u64, u64_array);

  // Test f64 array
  std::vector<f64> f64_array = {1.234, 5.678};
  vec v19{
      0xac, 0x02,
      0xa9, 0x3F, 0xF3, 0xBE, 0x76, 0xC8, 0xB4, 0x39, 0x58, // 1.234
      0xa9, 0x40, 0x16, 0xB6, 0x45, 0xA1, 0xCA, 0xC0, 0x83  // 5.678
  };
  vec bytes19 = pack109::serialize(f64_array);
  testvec("Test 27 - f64 array ser", bytes19, v19);

  std::vector<f64> deserialized_f64 = pack109::deserialize_vec_f64(bytes19);
  testvec<f64>("Test 28 - f64 array de", deserialized_f64, f64_array);

  // Test string array with elements "a" and "dd"
  std::vector<string> str_array = {"a", "dd"};
  vec v20{
      0xac, 0x02,          // Array tag (A8) and length = 2
      0xaa, 0x01, 'a',     // String "a" (S8 tag, length 1)
      0xaa, 0x02, 'd', 'd' // String "dd" (S8 tag, length 2)
  };
  vec bytes20 = pack109::serialize(str_array);
  testvec("Test 29 - string array ser", bytes20, v20);

  std::vector<string> deserialized_str_vec = pack109::deserialize_vec_string(bytes20);
  testvec("Test 30 - string array de", deserialized_str_vec, str_array);

  // Add after array tests
  std::map<string, u8> simple_map = {{"k", 0x42}};
  vec v21{
      0xae, 0x01,          // M8 tag + 1 pair
      0xaa, 0x01, 'k',     // Key: "k"
      0xa2, 0x42           // Value: 0x42
  };
  vec bytes21 = pack109::serialize(simple_map);
  testvec("Test 31 - map ser", bytes21, v21);

  std::map<string, u8> deserialized_map = pack109::deserialize_map_u8(bytes21);
  test("Test 32 - map de", deserialized_map["k"] == 0x42 ? 1 : 0, 1);

  return 0;
}
