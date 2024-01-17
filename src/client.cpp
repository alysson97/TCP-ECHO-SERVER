#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include "common.h"
#include "client.h"

using boost::asio::ip::tcp;



int main() {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);

        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("localhost", "12345"));

        // Send Login Request
        // (You need to set valid username and password)
        const char* username = "testuser";
        const char* password = "testpass";

        LoginRequest loginRequest{};
        loginRequest.header = { sizeof(LoginRequest), LOGIN_REQUEST, 0 };
        std::memcpy(loginRequest.username, username, std::strlen(username));
        std::memcpy(loginRequest.password, password, std::strlen(password));

        boost::asio::write(socket, boost::asio::buffer(&loginRequest, sizeof(LoginRequest)));

        // Receive Login Response
        LoginResponse loginResponse;
        boost::asio::read(socket, boost::asio::buffer(&loginResponse, sizeof(LoginResponse)));

        if (loginResponse.statusCode == 1) {
            std::cout << "Login successful!\n";

            // Send Echo Request
            std::string message = "Hello, Server!";
            EchoRequest echoRequest{};
            echoRequest.header = { static_cast<uint16_t>(sizeof(EchoRequest) + message.size()), ECHO_REQUEST, 0 };
            echoRequest.messageSize = static_cast<uint16_t>(message.size());
            echoRequest.cipherMessage.resize(echoRequest.messageSize - sizeof(EchoRequest) + sizeof(std::vector<uint8_t>));
            std::memcpy(echoRequest.cipherMessage.data(), message.c_str(), message.size());

            boost::asio::write(socket, boost::asio::buffer(&echoRequest, sizeof(EchoRequest)));
            boost::asio::write(socket, boost::asio::buffer(echoRequest.cipherMessage.data(), message.size()));

            // Receive Echo Response
            EchoResponse echoResponse;
            boost::asio::read(socket, boost::asio::buffer(&echoResponse, sizeof(EchoResponse)));
            std::cout << "Received Echo Response: " << std::string(echoResponse.plainMessage.begin(), echoResponse.plainMessage.end()) << "\n";

        }
        else {
            std::cout << "Login failed.\n";
        }

    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
