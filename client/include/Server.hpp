#pragma once

#include "HTTPRequest.hpp"
#include "Parsers/JSON.hpp"
#include <iostream>

// for convenience
using json = nlohmann::json;

class Server {
    public:
        Server(std::string p_server_port);
        json get(std::string p_url);
    private:
        std::string server_port, api_url = "http://127.0.0.1";
};