#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <csignal>
#include "pack109.hpp"
#include "program.hpp"
#include "hashmap.hpp"

/// In-memory file storage.
HashMap file_storage;
/// Path to persistence file, if enabled.
std::string persistence_file = "";
/// Server socket file descriptor.
int server_fd = -1;
/// Server running flag.
bool running = true;

/**
 * Signal handler for graceful shutdown.
 * 
 * Closes the server socket and sets the running flag to false.
 * @param signal The received signal number.
 */
void signal_handler(int signal) {
    std::cout << "\nReceived shutdown signal. Cleaning up..." << std::endl;
    running = false;
    if (server_fd != -1) {
        close(server_fd); // Unblock accept()
        server_fd = -1;
    }
}

/**
 * Saves the hash map to disk.
 * 
 * @param storage The file storage hash map.
 * @param filename The persistence file path.
 * @return true if successful, false otherwise.
 */
bool save_storage_to_disk(const HashMap& storage, const std::string& filename) {
    if (filename.empty()) return false;
    try {
        std::ofstream outfile(filename, std::ios::binary | std::ios::out);
        if (!outfile.is_open()) return false;
        std::vector<std::string> keys = storage.keys();
        uint32_t num_files = keys.size();
        outfile.write(reinterpret_cast<char*>(&num_files), sizeof(num_files));
        for (const auto& key : keys) {
            File file = storage.get(key);
            uint32_t filename_len = file.filename.length();
            outfile.write(reinterpret_cast<char*>(&filename_len), sizeof(filename_len));
            outfile.write(file.filename.c_str(), filename_len);
            uint32_t data_len = file.data.size();
            outfile.write(reinterpret_cast<char*>(&data_len), sizeof(data_len));
            outfile.write(reinterpret_cast<const char*>(file.data.data()), data_len);
        }
        outfile.close();
        std::cout << "Saved " << num_files << " files to disk: " << filename << std::endl;
        return true;
    } catch (...) {
        return false;
    }
}

/**
 * Loads the hash map from disk.
 * 
 * @param storage The file storage hash map to populate.
 * @param filename The persistence file path.
 * @return true if successful or file does not exist, false otherwise.
 */
bool load_storage_from_disk(HashMap& storage, const std::string& filename) {
    if (filename.empty()) return false;
    try {
        std::ifstream infile(filename, std::ios::binary | std::ios::in);
        if (!infile.is_open()) return true; // Not an error if file doesn't exist
        uint32_t num_files;
        infile.read(reinterpret_cast<char*>(&num_files), sizeof(num_files));
        for (uint32_t i = 0; i < num_files; i++) {
            uint32_t filename_len;
            infile.read(reinterpret_cast<char*>(&filename_len), sizeof(filename_len));
            std::string filename(filename_len, ' ');
            infile.read(&filename[0], filename_len);
            uint32_t data_len;
            infile.read(reinterpret_cast<char*>(&data_len), sizeof(data_len));
            std::vector<unsigned char> data(data_len);
            infile.read(reinterpret_cast<char*>(data.data()), data_len);
            File file(filename, data);
            storage.insert(filename, file);
        }
        infile.close();
        std::cout << "Loaded " << num_files << " files from disk: " << filename << std::endl;
        return true;
    } catch (...) {
        return false;
    }
}

/**
 * Receives all bytes requested from a socket.
 * 
 * @param sockfd The socket file descriptor.
 * @param data The buffer to fill.
 * @param length The number of bytes to receive.
 * @return true if all bytes are received, false otherwise.
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
 * Sends all bytes requested over a socket.
 * 
 * @param sockfd The socket file descriptor.
 * @param data The buffer to send.
 * @param length The number of bytes to send.
 * @return true if all bytes are sent, false otherwise.
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
 * Main server entry point.
 * 
 * Parses command-line arguments, sets up persistence, handles signals,
 * and runs the main server loop to accept and process client connections.
 * 
 * @return int Exit status code.
 */
int main(int argc, char *argv[]) {
    // Defaults
    std::string hostname = "localhost";
    int port = 8082;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--hostname" || arg == "-h") {
            if (i + 1 < argc) {
                auto host_port = parse_hostname(argv[++i]);
                hostname = host_port.first;
                port = host_port.second;
            } else {
                std::cerr << "Missing value for --hostname" << std::endl;
                return 1;
            }
        } else if (arg == "--persist" || arg == "-p") {
            if (i + 1 < argc) {
                persistence_file = argv[++i];
            } else {
                std::cerr << "Missing value for --persist" << std::endl;
                return 1;
            }
        }
    }

    // Load storage if needed
    if (!persistence_file.empty()) {
        if (!load_storage_from_disk(file_storage, persistence_file)) {
            std::cerr << "Failed to load storage from disk" << std::endl;
            return 1;
        }
    }

    // Register signal handler
    std::signal(SIGINT, signal_handler);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { std::cerr << "Error creating socket" << std::endl; return 1; }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error binding socket to port " << port << std::endl;
        close(server_fd); return 1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error listening for connections" << std::endl;
        close(server_fd); return 1;
    }

    std::cout << "Server started on " << hostname << ":" << port << std::endl;
    if (!persistence_file.empty()) std::cout << "Using persistence file: " << persistence_file << std::endl;

    while (running) {
        struct sockaddr_in client_address;
        socklen_t client_addrlen = sizeof(client_address);
        int client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_addrlen);
        if (client_socket < 0) {
            if (!running) break;
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // === CHANGE: Increased message size limit to 70000 bytes ===
        // Receive length prefix
        uint32_t msg_len = 0;
        if (!recv_all(client_socket, reinterpret_cast<unsigned char*>(&msg_len), sizeof(msg_len))) {
            std::cerr << "Error reading message length" << std::endl;
            close(client_socket); continue;
        }
        msg_len = ntohl(msg_len);

        // Updated size limit to accommodate serialization overhead
        // 65535 bytes for the file + max 1024 bytes overhead for serialization
        if (msg_len > 70000) {
            std::cerr << "Message too large: " << msg_len << " bytes" << std::endl;
            close(client_socket); continue;
        }

        std::vector<unsigned char> buffer(msg_len);
        if (!recv_all(client_socket, buffer.data(), msg_len)) {
            std::cerr << "Error reading message" << std::endl;
            close(client_socket); continue;
        }

        xor_crypt(buffer, XOR_KEY);

        std::vector<unsigned char> response;
        try {
            if (buffer[0] == FILE_MESSAGE) {
                File file = deserialize_file(buffer);
                // === CHANGE: Added debug output for file reception ===
                std::cout << "Received file: " << file.filename << " (" << file.data.size() << " bytes)" << std::endl;
                // === END CHANGE ===
                file_storage.insert(file.filename, file);
                Status status(STATUS_OK, "File received successfully");
                response = serialize_status(status);
            } else if (buffer[0] == REQUEST_MESSAGE) {
                Request request = deserialize_request(buffer);
                // === CHANGE: Added debug output for file request ===
                std::cout << "File requested: " << request.filename << std::endl;
                // === END CHANGE ===
                if (file_storage.contains(request.filename)) {
                    File file = file_storage.get(request.filename);
                    // === CHANGE: Added debug output for file sending ===
                    std::cout << "Sending file: " << file.filename << " (" << file.data.size() << " bytes)" << std::endl;
                    // === END CHANGE ===
                    response = serialize_file(file);
                } else {
                    Status status(STATUS_FILE_NOT_FOUND, "File not found");
                    response = serialize_status(status);
                }
            } else {
                Status status(STATUS_ERROR, "Unknown message type");
                response = serialize_status(status);
            }
        } catch (const std::exception& e) {
            Status status(STATUS_ERROR, e.what());
            response = serialize_status(status);
        }

        xor_crypt(response, XOR_KEY);

        // Send length prefix
        uint32_t resp_len = htonl(response.size());
        if (!send_all(client_socket, reinterpret_cast<unsigned char*>(&resp_len), sizeof(resp_len))) {
            std::cerr << "Error sending response length" << std::endl;
            close(client_socket); continue;
        }
        if (!send_all(client_socket, response.data(), response.size())) {
            std::cerr << "Error sending response" << std::endl;
            close(client_socket); continue;
        }
        close(client_socket);

        // Save to disk if persistence enabled
        if (!persistence_file.empty()) save_storage_to_disk(file_storage, persistence_file);
    }

    // Save storage to disk before shutting down
    if (!persistence_file.empty()) save_storage_to_disk(file_storage, persistence_file);

    std::cout << "Server shut down" << std::endl;
    close(server_fd);
    return 0;
}
