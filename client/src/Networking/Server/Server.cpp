#include "Networking/Server/Server.hpp"

// for convenience
using json = nlohmann::json;

Server::Server(std::string p_server_port) :server_port(p_server_port) {};

std::string Server::getApiURL(std::string p_url) {
    return api_url + ":" + server_port + "/" + p_url;
};

http::Request Server::getRequestFromURL(std::string p_url) {
    const std::string full_url = getApiURL(p_url);
    http::Request request{full_url};

    return request;
}

json Server::safeReturnResponse(http::Response p_response) {
    try {
        const std::string response_str = std::string{p_response.body.begin(), p_response.body.end()};
        return json::parse(response_str);
    }catch (...) {
        return NULL;
    }
}

json Server::get(std::string p_url) {
    http::Request request = getRequestFromURL(p_url);
	http::Response response = request.send("GET");

    return safeReturnResponse(response);
};

json Server::post(std::string p_url, json p_body) {
    http::Request request = getRequestFromURL(p_url);

    std::string body = p_body.dump();
    http::Response response = request.send("POST", body, {"Content-Type: application/json"});

    return safeReturnResponse(response);
};