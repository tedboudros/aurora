#include "Server.hpp"
#include "HTTPRequest.hpp"
#include "Parsers/JSON.hpp"

// for convenience
using json = nlohmann::json;

Server::Server(std::string p_server_port) :server_port(p_server_port) {

};

json Server::get(std::string p_url) {
    const std::string full_url = api_url + ":" + server_port + "/" + p_url;
    http::Request request{full_url};

	const auto response = request.send("GET");
	const std::string response_str = std::string{response.body.begin(), response.body.end()};

    return json::parse(response_str);
};