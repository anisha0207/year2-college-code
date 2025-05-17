#include <iostream>
#include <cassert>
#include <vector>
#include "pack109.hpp"
#include "program.hpp"
#include "hashmap.hpp"

//formatting for all test cases below are the same, it was important to incorporate program.hpp to call functions when testing deserializing and serializing within request, file and status
//assert() was an important function to use from to verify that the program's state matches the expected outcome after serialization and deserialization operations. If an assertion fails (i.e., the condition is false), the program immediately terminates and prints an error message
//I had AI help writing out three different tests ({0xA0}, {0x00, 0x00, 0x00}, {0x10, 0x20}) which each scenerio is checked through assert() to validate a malformed file, status, and request

/**
 * Compares two byte vectors for equality.
 * 
 * @param a First vector to compare.
 * @param b Second vector to compare.
 * @return true if vectors are equal, false otherwise.
 */
bool compare_vectors(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    return a == b;
}

/**
 * Test serialization and deserialization of the File structure.
 * 
 * This test ensures that a File object can be serialized and then deserialized
 * back to an object with the same data.
 */
void test_file_serialization() {
    std::cout << "1. Testing File serialization" << std::endl;
    File f = {.filename = "notes.txt", .data = {'a', 'b', 'c'}};
    std::vector<unsigned char> encoded = serialize_file(f);
    File decoded = deserialize_file(encoded);
    assert(decoded.filename == f.filename);
    assert(decoded.data == f.data);
    std::cout << "Passed" << std::endl;
}

/**
 * Test serialization and deserialization of the Request structure.
 * 
 * This test ensures that a Request object can be serialized and then deserialized
 * back to an object with the same data.
 */
void test_request_serialization() {
    std::cout << "2. Testing Request serialization" << std::endl;
    Request r = {.filename = "file.txt"};
    std::vector<unsigned char> encoded = serialize_request(r);
    Request decoded = deserialize_request(encoded);
    assert(decoded.filename == r.filename);
    std::cout << "Passed" << std::endl;
}

/**
 * Test serialization and deserialization of the Status structure.
 * 
 * This test ensures that a Status object can be serialized and then deserialized
 * back to an object with the same data.
 */
void test_status_serialization() {
    std::cout << "3. Testing Status serialization" << std::endl;
    Status s = Status(STATUS_OK, "SUCCESS");
    std::vector<unsigned char> encoded = serialize_status(s);
    Status decoded = deserialize_status(encoded);
    assert(decoded.message == s.message);
    assert(decoded.code == s.code);
    std::cout << "Passed" << std::endl;
}

/**
 * Test serialization and deserialization with edge case values.
 * 
 * This test covers scenarios such as empty filenames, empty data vectors,
 * and empty messages to ensure robustness.
 */
void test_edge_cases() {
    std::cout << "4. Testing edge cases" << std::endl;

    File f1 = {.filename = "", .data = {}};
    File decoded_f1 = deserialize_file(serialize_file(f1));
    assert(decoded_f1.filename == "");
    assert(decoded_f1.data.empty());

    Request r = {.filename = ""};
    Request decoded_r = deserialize_request(serialize_request(r));
    assert(decoded_r.filename == "");

    Status s = Status(STATUS_OK, "");
    Status decoded_s = deserialize_status(serialize_status(s));
    assert(decoded_s.message == "");
    assert(decoded_s.code == s.code);

    std::cout << "Edge case tests passed" << std::endl;
}

/**
 * Test deserialization with malformed or invalid input data.
 * 
 * This test ensures that the deserialization functions properly handle
 * malformed or incomplete data by throwing exceptions or errors.
 */
void test_malformed_inputs() {
    std::cout << "5. Testing malformed messages" << std::endl;

    // Too short
    try {
        std::vector<uint8_t> bad = {0xA0}; // fake tag
        File f = deserialize_file(bad);
        assert(false); // Should not reach here
    } catch (...) {
        std::cout << "Caught short/malformed file: Pass" << std::endl;
    }

    try {
        std::vector<uint8_t> bad = {0x00, 0x00, 0x00};//invalid tag
        Request r = deserialize_request(bad);
        assert(false);
    } catch (...) {
        std::cout << "Caught malformed request: Pass" << std::endl;
    }

    try {
        std::vector<uint8_t> bad = {0x10, 0x20}; // Garbage
        Status s = deserialize_status(bad);
        assert(false);
    } catch (...) {
        std::cout << "Caught malformed status: Pass" << std::endl;
    }
}
/**
 * @brief Test serialization and deserialization of a File structure with maximum allowed data size.
 *
 * This test ensures that a File object with a data vector of 65,535 bytes can be serialized and deserialized correctly.
 */
void test_large_file_serialization() {
    std::cout << "6. Testing Large File serialization" << std::endl;
    std::vector<uint8_t> large_data(65535, 'x'); // 65,535 bytes of 'x'
    File f = {.filename = "maxfile.bin", .data = large_data};
    std::vector<unsigned char> encoded = serialize_file(f);
    File decoded = deserialize_file(encoded);
    assert(decoded.filename == f.filename);
    assert(decoded.data == f.data);
    std::cout << "Passed" << std::endl;
}
/**
 * @brief Test serialization and deserialization of a Request with a large filename.
 *
 * This test checks that the system can handle a Request message with a filename of significant length.
 */
void test_large_request_serialization() {
    std::cout << "7. Testing Large Request serialization" << std::endl;
    std::string large_filename(255, 'a'); // 255 is a common max filename length
    Request r = {.filename = large_filename};
    std::vector<unsigned char> encoded = serialize_request(r);
    Request decoded = deserialize_request(encoded);
    assert(decoded.filename == r.filename);
    std::cout << "Passed" << std::endl;
}
/**
 * @brief Test serialization and deserialization of a Status with a large message.
 *
 * This test ensures that a Status message with a long message string can be processed correctly.
 */
void test_large_status_serialization() {
    std::cout << "8. Testing Large Status serialization" << std::endl;
    std::string large_message(1024, 'z'); // 1 KB message
    Status s = Status(STATUS_OK, large_message);
    std::vector<unsigned char> encoded = serialize_status(s);
    Status decoded = deserialize_status(encoded);
    assert(decoded.message == s.message);
    assert(decoded.code == s.code);
    std::cout << "Passed" << std::endl;
}
/**
 * @brief Test serialization/deserialization of a File with a large data vector and empty filename.
 *
 * Ensures edge cases are handled even at maximum data size.
 */
void test_large_file_empty_filename() {
    std::cout << "9. Testing Large File with empty filename" << std::endl;
    std::vector<uint8_t> large_data(65535, 'x');
    File f = {.filename = "", .data = large_data};
    File decoded = deserialize_file(serialize_file(f));
    assert(decoded.filename == "");
    assert(decoded.data == f.data);
    std::cout << "Passed" << std::endl;
}
/**
 * @brief Test deserialization of a malformed File message with large data.
 *
 * Ensures the system detects and handles malformed messages, even when large.
 */
void test_malformed_large_file() {
    std::cout << "10. Testing Malformed Large File message" << std::endl;
    // Create an incomplete message (missing some bytes)
    std::vector<uint8_t> bad(65534, 'x'); // One byte short
    try {
        File f = deserialize_file(bad);
        assert(false); // Should not reach here
    } catch (...) {
        std::cout << "Caught malformed large file: Pass" << std::endl;
    }
}

/**
 * Main function to run all test cases.
 * 
 * Executes all serialization and deserialization test cases above and prints results.
 * 
 * @return int Returns 0 on successful completion.
 */
int main() {
    test_file_serialization();
    test_request_serialization();
    test_status_serialization();
    test_edge_cases();
    test_malformed_inputs();
    test_large_file_serialization();
    test_large_request_serialization();
    test_large_status_serialization();
    test_large_file_empty_filename();
    test_malformed_large_file();
    std::cout << "\nALL TESTS PASSED\n" << std::endl;
    return 0;
}
