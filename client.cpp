#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include "pack109.hpp"
#include "program.hpp"

/**
 * Reads an entire file into a byte vector.
 * 
 * @param filename The name of the file to read.
 * @return std::vector<unsigned char> The file contents as a byte vector.
 * @throws std::runtime_error if the file cannot be opened.
 */
std::vector<unsigned char> read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Failed to open file: " + filename);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> content(size);
    file.read(reinterpret_cast<char*>(content.data()), size);
    return content;
}

/**
 * Writes a byte vector to a file.
 * 
 * @param filename The name of the file to write.
 * @param content The contents to write as a byte vector.
 * @throws std::runtime_error if the file cannot be created.
 */
void write_file(const std::string& filename, const std::vector<unsigned char>& content) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Failed to create file: " + filename);
    file.write(reinterpret_cast<const char*>(content.data()), content.size());
}

/**
 * Parses a hostname string with optional port (format: host:port).
 * 
 * @param hostname The hostname string to parse.
 * @return std::pair<std::string, int> The host and port as a pair.
 */
std::pair<std::string, int> parse_hostname(const std::string& hostname) {
    size_t colon_pos = hostname.find(':');
    if (colon_pos == std::string::npos) return std::make_pair(hostname, 8081);
    std::string host = hostname.substr(0, colon_pos);
    int port = std::stoi(hostname.substr(colon_pos + 1));
    return std::make_pair(host, port);
}

/**
 * Sends all data in the buffer, looping as needed.
 * 
 * @param sockfd The socket file descriptor.
 * @param data Pointer to the data buffer.
 * @param length Number of bytes to send.
 * @return true if all data is sent, false otherwise.
 */
bool send_all(int sockfd, const unsigned char* data, size_t length) {
    size_t total_sent = 0;
    while (total_sent < length) {
        ssize_t sent = send(sockfd, data + total_sent, length - total_sent, 0);
        if (sent <= 0) return false;
        total_sent += sent;
    }
    return true;
}

/**
 * Receives all data into buffer, looping as needed.
 * 
 * @param sockfd The socket file descriptor.
 * @param data Pointer to the buffer to fill.
 * @param length Number of bytes to receive.
 * @return true if all data is received, false otherwise.
 */
bool recv_all(int sockfd, unsigned char* data, size_t length) {
    size_t total_received = 0;
    while (total_received < length) {
        ssize_t received = recv(sockfd, data + total_received, length - total_received, 0);
        if (received <= 0) return false;
        total_received += received;
    }
    return true;
}

/**
 * Entry point for the client program.
 * 
 * Parses command-line arguments, connects to the server, sends a file or file request,
 * and processes the server's response.
 * 
 * Command line options:
 *   --hostname <host[:port]>  Specify the server host and optional port (default: localhost:8081)
 *   --send <filename>         Send a file to the server
 *   --request <filename>      Request a file from the server
 * 
 * @return int Exit status code.
 */
int main(int argc, char *argv[]) {
    std::string hostname = "localhost";
    int port = 8081;
    std::string send_file = "";
    std::string request_file = "";

    // Parse args
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--hostname") {
            if (i + 1 < argc) {
                auto host_port = parse_hostname(argv[++i]);
                hostname = host_port.first;
                port = host_port.second;
            } else { std::cerr << "Missing value for --hostname\n"; return 1; }
        } else if (arg == "--send") {
            if (i + 1 < argc) send_file = argv[++i];
            else { std::cerr << "Missing value for --send\n"; return 1; }
        } else if (arg == "--request") {
            if (i + 1 < argc) request_file = argv[++i];
            else { std::cerr << "Missing value for --request\n"; return 1; }
        }
    }
    if (send_file.empty() && request_file.empty()) {
        std::cerr << "Error: Either --send or --request must be specified\n";
        return 1;
    }

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) { std::cerr << "Error creating socket\n"; return 1; }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, hostname.c_str(), &server_addr.sin_addr) <= 0) {
        if (hostname == "localhost") server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        else { std::cerr << "Invalid address: " << hostname << '\n'; close(client_fd); return 1; }
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed to " << hostname << ":" << port << '\n';
        close(client_fd); return 1;
    }
    std::cout << "Connected to server at " << hostname << ":" << port << std::endl;

    try {
        std::vector<unsigned char> message;
        if (!send_file.empty()) {
            std::vector<unsigned char> file_content = read_file(send_file);

            // === CHANGE: File size check added ===
            if (file_content.size() > 65535) {
                std::cerr << "File too large: " << file_content.size() << " bytes (max 65535)\n";
                close(client_fd); return 1;
            }

            File file;
            file.filename = send_file;
            file.data = file_content;
            message = serialize_file(file);
            xor_crypt(message, XOR_KEY);
            std::cout << "Sending file: " << send_file << " (" << file_content.size() << " bytes)" << std::endl;
            std::cout << "Serialized message size: " << message.size() << " bytes" << std::endl;
        } else {
            Request request;
            request.filename = request_file;
            message = serialize_request(request);
            xor_crypt(message, XOR_KEY);
            std::cout << "Requesting file: " << request_file << std::endl;
        }

        // Length prefix (4 bytes, network byte order)
        uint32_t msg_len = htonl(message.size());
        if (!send_all(client_fd, reinterpret_cast<unsigned char*>(&msg_len), sizeof(msg_len))) {
            std::cerr << "Error sending message length\n"; close(client_fd); return 1;
        }
        if (!send_all(client_fd, message.data(), message.size())) {
            std::cerr << "Error sending message to server\n"; close(client_fd); return 1;
        }

        // Receive length prefix
        uint32_t resp_len = 0;
        if (!recv_all(client_fd, reinterpret_cast<unsigned char*>(&resp_len), sizeof(resp_len))) {
            std::cerr << "Error reading response length from server\n"; close(client_fd); return 1;
        }
        resp_len = ntohl(resp_len);

        // === CHANGE: Updated response size check for clarity ===
        if (resp_len > 70000) {  // Updated to match server limit (could also use 65535)
            std::cerr << "Response too large: " << resp_len << " bytes\n"; close(client_fd); return 1;
        }

        std::vector<unsigned char> buffer(resp_len);
        if (!recv_all(client_fd, buffer.data(), resp_len)) {
            std::cerr << "Error reading response from server\n"; close(client_fd); return 1;
        }
        xor_crypt(buffer, XOR_KEY);

        if (buffer[0] == FILE_MESSAGE) {
            File received_file = deserialize_file(buffer);
            write_file(received_file.filename, received_file.data);
            std::cout << "Received file: " << received_file.filename 
                      << " (" << received_file.data.size() << " bytes)\nFile saved successfully\n";
        } else if (buffer[0] == STATUS_MESSAGE) {
            Status status = deserialize_status(buffer);
            if (status.code == STATUS_OK)
                std::cout << "Server response: SUCCESS - " << status.message << std::endl;
            else
                std::cerr << "Server response: ERROR - " << status.message << std::endl;
        } else {
            std::cerr << "Unknown response type from server" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    close(client_fd);
    return 0;
}
