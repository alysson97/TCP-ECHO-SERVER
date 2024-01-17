#include <iostream>
#include <thread>
#include <vector>
#include <cstdint>
#include <boost/asio.hpp>
#include "common.h"
#include "server.h"

using boost::asio::ip::tcp;



int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));

        std::vector<std::thread> threads;
        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            threads.emplace_back(handleClient, std::move(socket));
        }

        // Join threads when the application exits
        for (auto& thread : threads) {
            thread.join();
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}