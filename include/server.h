#ifndef SERVER_H
#define SERVER_H

#include "common.h"

// LCG (Linear Congruential Generator) for key generation
uint32_t next_key(uint32_t key) {
    return (key * 1103515245 + 12345) % 0x7FFFFFFF;
}

// Decrypt Echo Request cipher message
std::vector<uint8_t> decryptEchoRequest(const EchoRequest& echoRequest) {
    // Access the username and password directly from EchoRequest
    const char* username = echoRequest.cipherMessage.size() >= 64 ? reinterpret_cast<const char*>(echoRequest.cipherMessage.data()) : nullptr;
    const char* password = echoRequest.cipherMessage.size() >= 64 + 32 ? reinterpret_cast<const char*>(echoRequest.cipherMessage.data() + 64) : nullptr;

    // Calculate the checksums of username and password
    uint8_t username_sum = 0;
    uint8_t password_sum = 0;

    for (int i = 0; i < 32; ++i) {
        username_sum += static_cast<uint8_t>(username[i]);
        password_sum += static_cast<uint8_t>(password[i]);
    }

    // Calculate the initial key
    uint32_t initial_key = (static_cast<uint32_t>(echoRequest.header.messageSequence) << 16) |
        (static_cast<uint32_t>(username_sum) << 8) |
        (static_cast<uint32_t>(password_sum));

    // Generate the cipher key
    uint32_t cipher_key[64];
    cipher_key[0] = next_key(initial_key) % 256;

    for (int i = 1; i < 64; ++i) {
        cipher_key[i] = next_key(cipher_key[i - 1]) % 256;
    }

    // XOR cipher text and cipher key to get plain text
    std::vector<uint8_t> plainMessage(echoRequest.cipherMessage.size());

    for (size_t i = 0; i < echoRequest.cipherMessage.size(); ++i) {
        plainMessage[i] = echoRequest.cipherMessage[i] ^ static_cast<uint8_t>(cipher_key[i % 64]);
    }

    return plainMessage;
}

// Function to handle Echo Request
void handleEchoRequest(tcp::socket& socket, const EchoRequest& request) {
    // Decrypt the message
    std::vector<uint8_t> plainMessage = decryptEchoRequest(request);

    // Prepare Echo Response
    EchoResponse response{ request.header, static_cast<uint16_t>(plainMessage.size()), plainMessage };
    response.header.messageSize = sizeof(EchoResponse) - sizeof(std::vector<uint8_t>) + plainMessage.size();
    try {
        boost::asio::write(socket, boost::asio::buffer(&response, sizeof(EchoResponse)));
    }
    catch (std::exception& e) {
        std::cerr << "Error writing LoginResponse to socket: " << e.what() << "\n";
    }

}

// Function to handle Login Request
void handleLoginRequest(tcp::socket& socket, const LoginRequest& request) {
    bool isValid = (std::strlen(request.username) > 0) && (std::strlen(request.password) > 0);

    LoginResponse response{ request.header, isValid ? 1 : 0 };  // OK status code if valid, FAILED otherwise
    try {
        boost::asio::write(socket, boost::asio::buffer(&response, sizeof(LoginResponse)));
    }
    catch (std::exception& e) {
        std::cerr << "Error writing LoginResponse to socket: " << e.what() << "\n";
    }
}

// Function to handle a connected client
void handleClient(tcp::socket socket) {
    try {
        for (;;) {
            Header header;
            boost::asio::read(socket, boost::asio::buffer(&header, sizeof(Header)));

            switch (header.messageType) {
            case LOGIN_REQUEST: {
                LoginRequest loginRequest;
                boost::asio::read(socket, boost::asio::buffer(&loginRequest, sizeof(LoginRequest)));
                handleLoginRequest(socket, loginRequest);
                break;
            }
            case ECHO_REQUEST: {
                EchoRequest echoRequest;
                boost::asio::read(socket, boost::asio::buffer(&echoRequest, sizeof(EchoRequest) + sizeof(std::vector<uint8_t>))); //or -
                echoRequest.cipherMessage.resize(echoRequest.messageSize - sizeof(EchoRequest) + sizeof(std::vector<uint8_t>));
                boost::asio::read(socket, boost::asio::buffer(echoRequest.cipherMessage));
                handleEchoRequest(socket, echoRequest);
                break;
            }
            default:
                std::cerr << "Unknown message type: " << static_cast<int>(header.messageType) << "\n";
                break;
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error reading from socket: " << e.what() << "\n";
    }
}

#endif