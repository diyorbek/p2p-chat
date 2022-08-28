#include <boost/asio.hpp>
#include <iostream>

#include "peer.h"
#include "server.h"

using boost::asio::ip::udp;

server::server(boost::asio::io_service& io_service, short port)
    : socket(io_service, udp::endpoint(udp::v4(), port)) {}

void server::start() {
  while (true) {
    char data[max_buffer_size];
    udp::endpoint remote_endpoint;
    boost::system::error_code error;

    size_t length = socket.receive_from(
        boost::asio::buffer(data, max_buffer_size), remote_endpoint, 0, error);

    if (error && error != boost::asio::error::message_size)
      std::cerr << "Exception: " << error.what() << "\n";

    handle_request(remote_endpoint, std::string(data, length));
  }
}

void server::handle_request(udp::endpoint remote_endpoint,
                            std::string raw_request) {
  auto address = remote_endpoint.address().to_string();
  auto port = remote_endpoint.port();

  if (sessions.count({address, port})) {
    return;
  }

  auto new_peer = peer::deserilize(remote_endpoint, raw_request);
  auto room_name = new_peer.room_name;

  sessions.insert({{address, port}, room_name});

  if (hosts.count(room_name) == 0) {
    hosts.emplace(room_name, new_peer);

    return;
  }

  auto peer1 = hosts[room_name];
  auto peer2 = new_peer;
  auto peer1_data = peer1.serialize();
  auto peer2_data = peer2.serialize();

  socket.send_to(boost::asio::buffer(peer2_data), peer1.remote_endpoint);
  socket.send_to(boost::asio::buffer(peer1_data), peer2.remote_endpoint);

  sessions.erase({peer1.ip, peer1.port});
  sessions.erase({peer2.ip, peer2.port});
  hosts.erase(room_name);
}