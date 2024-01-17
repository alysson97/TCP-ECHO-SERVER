#ifndef COMMON_H
#define COMMON_H

// Define message types
enum MessageType : uint8_t {
    LOGIN_REQUEST = 0,
    LOGIN_RESPONSE = 1,
    ECHO_REQUEST = 2,
    ECHO_RESPONSE = 3
};

// Define header structure
struct Header {
    uint16_t messageSize;
    uint8_t messageType;
    uint8_t messageSequence;
};

// Define login request structure
struct LoginRequest {
    Header header;
    char username[32];
    char password[32];
};

// Define login response structure
struct LoginResponse {
    Header header;
    uint16_t statusCode;
};

// Define echo request structure
struct EchoRequest {
    Header header;
    uint16_t messageSize;
    std::vector<uint8_t> cipherMessage;
};

// Define echo response structure
struct EchoResponse {
    Header header;
    uint16_t messageSize;
    std::vector<uint8_t> plainMessage;
};

#endif