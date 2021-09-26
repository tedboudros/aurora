#pragma once

#include "Networking/HTTPRequest/HTTPRequest.hpp"
#include "Networking/JSON/JSON.hpp"

#include <iostream>

// for convenience
using json = nlohmann::json;

class Server {
    public:
        Server(std::string p_server_port);
        json get(std::string p_url);
        json post(std::string p_url, json p_body);
    private:
        std::string server_port, api_url = "http://127.0.0.1";
        std::string getApiURL(std::string p_url);
        http::Request getRequestFromURL(std::string p_url);
        json safeReturnResponse(http::Response p_response);
};