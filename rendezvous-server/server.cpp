#include <boost/asio.hpp>
#include <iostream>
#include <sstream>

#include "peer.h"
#include "server.h"

using boost::asio::ip::udp;

server::server(boost::asio::io_service& io_service, short port)
    : socket(io_service, udp::endpoint(udp::v4(), port)) {}

void server::start() {
  while (true) {
    auto received = receive();

    try {
      handle_request(received);
    } catch (std::exception& e) {
      std::cerr << "Failed to handle request: " << e.what() << "\n";
    }
  }
}

received server::receive() {
  char data[max_buffer_size];
  udp::endpoint remote_endpoint;
  boost::system::error_code error;

  size_t length = socket.receive_from(
      boost::asio::buffer(data, max_buffer_size), remote_endpoint, 0, error);

  if (error && error != boost::asio::error::message_size)
    throw std::runtime_error("Exception while receiving: " + error.what());

  send_receive_ack(remote_endpoint);

  packet pckt;
  pckt.deserialize(data, length);

  return {remote_endpoint, pckt};
}

void server::send_receive_ack(udp::endpoint remote_endpoint) {
  packet ack_response(ACK);
  send_to(remote_endpoint, ack_response);
}

void server::send_to(udp::endpoint remote_endpoint, packet packet) {
  auto data = packet.serialize();
  socket.send_to(boost::asio::buffer(data.first, data.second), remote_endpoint);
}

void server::handle_request(received received) {
  auto remote_endpoint = received.remote_endpoint;
  auto received_packet = received.packet;
  auto address = remote_endpoint.address().to_string();
  auto port = remote_endpoint.port();

  if (sessions.count({address, port})) {
    return;
  }

  auto new_peer =
      peer::deserilize(remote_endpoint, received_packet.get_content());
  auto room_name = new_peer.room_name;

  sessions.insert({{address, port}, room_name});

  if (hosts.count(room_name) == 0) {
    hosts.emplace(room_name, new_peer);

    return;
  }

  auto peer1 = hosts[room_name];
  auto peer2 = new_peer;
  packet peer1_data(PSH, peer1.serialize());
  packet peer2_data(PSH, peer2.serialize());

  send_to(peer1.remote_endpoint, peer2_data);
  send_to(peer2.remote_endpoint, peer1_data);

  sessions.erase({peer1.ip, peer1.port});
  sessions.erase({peer2.ip, peer2.port});
  hosts.erase(room_name);
}