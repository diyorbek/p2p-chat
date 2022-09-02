#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>

#include "client.h"
#include "utils.h"

using boost::asio::io_service;
using boost::asio::ip::address;
using boost::asio::ip::udp;

client::client(io_service& io_service)
    : io_svc(io_service), socket(io_service, udp::endpoint(udp::v4(), 0)) {}

void client::connect(peer_info& peer) {
  auto ip_address = address::from_string(peer.ip);
  udp::endpoint peer_endpoint(ip_address, stoi(peer.port));

  socket.connect(peer_endpoint);
  connected_peer = &peer;

  std::thread receiver_thread(&client::start_message_receive, this);
  start_message_send();
  receiver_thread.join();
}

std::string client::receive_message() {
  char buffer[max_length];
  size_t length = socket.receive(boost::asio::buffer(buffer, max_length));
  std::string received_message(buffer, length);
  boost::trim(received_message);

  return received_message;
}

void client::start_message_receive() {
  while (true) {
    std::string received_message = receive_message();

    if (received_message.empty())
      continue;

    utils::print_received_message(*connected_peer, received_message);
  }
}

void client::start_message_send() {
  while (true) {
    std::string message = utils::input_message();

    socket.send(boost::asio::buffer(message));
    utils::print_sent_message(message);
  }
}
