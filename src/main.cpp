#include <iostream>
#include <sstream>
#include <string>
#include <grpcpp/grpcpp.h>

#include "server.h"

// This is the main function of the service, it wil turn on the service and listen to the port
// provided by the user. It will also create a memory pool of the size provided by the user.
// The memory pool will be used to store the data that is sent to the service.
int main (int argc, char* argv[]) {
    int port = 0;
    int memsize = 0;
    std::string dumpFolder;

    if (argc < 7 || argc > 7) {
        std::cerr << "You provided not enough or too much arguments, try: " << argv[0] << " -port <port> -memsize <memsize> -dump_folder <dump_folder>" << std::endl;
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-port") {
            port = std::stoi(argv[i + 1]);
            ++i;
        } else if (std::string(argv[i]) == "-memsize") {
            memsize = std::stoi(argv[i + 1]);
            ++i;
        } else if (std::string(argv[i]) == "-dump_folder") {
            dumpFolder = argv[i + 1];
            ++i;
        } else {
            std::cerr << "Invalid argument: " << argv[i] << std::endl;
        }
    }

    if (port == 0 || memsize == 0 || dumpFolder.empty()) {
        std::cerr << "You did not provide all the arguments, try: " << argv[0] << " -port <port> -memsize <memsize> -dump_folder <dump_folder>" << std::endl;
        return 0;
    }

    std::string server_address;
    server_address = "0.0.0.0:" + std::to_string(port);

    starter starter(server_address, memsize, dumpFolder);
    starter.startServer();

    return 0;
}