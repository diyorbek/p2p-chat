#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/url/src.hpp>
#include <iostream>
#include <string>
#include <thread>
#include "registerer.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using namespace boost::urls;
using tcp = boost::asio::ip::tcp;

constexpr unsigned short PORT = 8081;

void handle_request(http::request<http::string_body> const& req,
                    http::response<http::string_body>& res,
                    register_request& data) {
  url_view uv(req.target());
  std::cout << uv.path() << std::endl;

  if (uv.path() != "/register") {
    res.result(http::status::not_found);
    res.set(http::field::content_type, "text/plain");
    res.body() = "Invalid request target";
    return;
  }

  if (req.method() != http::verb::get) {
    res.result(http::status::bad_request);
    res.set(http::field::content_type, "text/plain");
    res.body() = "Invalid request method";
    return;
  }

  if (uv.params().size() != 2 || !uv.params().contains("name") ||
      !uv.params().contains("room_name")) {
    res.result(http::status::bad_request);
    res.set(http::field::content_type, "text/plain");
    res.body() = "Invalid request params";
    return;
  }

  // Parse the query parameter
  auto name = (*uv.params().find("name")).value;
  auto room_name = (*uv.params().find("room_name")).value;

  if (data.name.empty() || data.room_name.empty()) {
    res.result(http::status::bad_request);
    res.set(http::field::content_type, "text/plain");
    res.body() = "Invalid request params";
    return;
  }

  data.name = name;
  data.room_name = room_name;

  // Build the response
  res.result(http::status::ok);
  res.set(http::field::content_type, "text/plain");
  res.body() = "Received request";
}

void http_session(tcp::socket socket, register_request& data) {
  try {
    beast::flat_buffer buffer;

    http::request<http::string_body> req;
    http::read(socket, buffer, req);

    http::response<http::string_body> res;
    handle_request(req, res, data);

    http::write(socket, res);
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

int start_http_server(register_request& data) {
  try {
    net::io_context ioc;
    tcp::acceptor acceptor{ioc, {tcp::v4(), PORT}};

    while (true) {
      tcp::socket socket{ioc};
      acceptor.accept(socket);
      http_session(std::move(socket), data);
    }
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
