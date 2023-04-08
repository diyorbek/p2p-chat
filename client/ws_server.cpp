#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json/src.hpp>
#include <iostream>
#include <string>
#include <thread>
#include "registerer.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

constexpr unsigned short PORT = 8080;

// function to serialize struct into json
std::string serialize(const register_request& request) {
  boost::json::object obj;
  obj["name"] = request.name;
  obj["room_name"] = request.room_name;

  boost::json::value value = obj;
  return boost::json::serialize(value);
}

void session(tcp::socket socket) {
  try {
    websocket::stream<tcp::socket> ws{std::move(socket)};
    ws.accept();

    while (true) {
      beast::flat_buffer buffer;
      ws.read(buffer);

      std::string received_message = beast::buffers_to_string(buffer.data());
      boost::trim(received_message);
      std::string json_response =
          R"({"response": "OK", "message": ")" + received_message + "\"})";

      ws.text(true);
      ws.write(net::buffer(json_response));
    }
  } catch (beast::system_error const& se) {
    if (se.code() != websocket::error::closed) {
      std::cerr << "Error: " << se.code().message() << std::endl;
    }
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

int start_ws() {
  try {
    register_request request{"test", "test"};
    std::string json_request = serialize(request);
    std::cout << json_request << std::endl;

    net::io_context ioc;
    tcp::acceptor acceptor{ioc, {tcp::v4(), PORT}};

    while (true) {
      tcp::socket socket{ioc};
      acceptor.accept(socket);
      std::thread session_thread(session, std::move(socket));
      session_thread.detach();
    }

    return 0;
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
