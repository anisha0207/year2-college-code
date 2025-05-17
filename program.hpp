#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <string>
#include <vector>

// Message types
#define FILE_MESSAGE 0x01
#define REQUEST_MESSAGE 0x02
#define STATUS_MESSAGE 0x03

// Status codes
#define STATUS_OK 200
#define STATUS_FILE_CREATED 201
#define STATUS_FILE_UPDATED 202
#define STATUS_FILE_NOT_FOUND 404
#define STATUS_ERROR 500
#define STATUS_MALFORMED_MESSAGE 400
#define STATUS_MEMORY_ERROR 507

// XOR encryption key
#define XOR_KEY 42 //key for testing in this assignment as highlighted

//important to define files, reuqust, and struct as basic functionalities and how they are incorporated throughout server and test files
// File struct
struct File {
    std::string filename;
    std::vector<unsigned char> data;
    
    File() = default;
    File(const std::string& name, const std::vector<unsigned char>& content)
        : filename(name), data(content) {}
};

// Request struct
struct Request {
    std::string filename;
    
    Request() = default;
    Request(const std::string& name) : filename(name) {}
};

// Status struct
struct Status {
    int code;
    std::string message;
    
    Status() = default;
    Status(int c, const std::string& msg) : code(c), message(msg) {}
};

// Function prototypes for serialization and deserialization
std::vector<unsigned char> serialize_file(const File& file);
File deserialize_file(const std::vector<unsigned char>& data);

std::vector<unsigned char> serialize_request(const Request& req);
Request deserialize_request(const std::vector<unsigned char>& data);

std::vector<unsigned char> serialize_status(const Status& status);
Status deserialize_status(const std::vector<unsigned char>& data);

// Helper function to convert bytes to string
std::string bytes_to_string(const std::vector<unsigned char>& byte_data);

// XOR encryption/decryption function (same for both operations)
void xor_crypt(std::vector<unsigned char>& data, unsigned char key);

#endif // PROGRAM_HPP