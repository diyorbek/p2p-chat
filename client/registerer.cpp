
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <vector>

#include "../shared/packet.h"
#include "client.h"
#include "peer_info.h"
#include "registerer.h"

#define REGISTERER_SERVER_IP "0.0.0.0"
#define REGISTERER_SERVER_PORT 8081

using boost::asio::ip::address;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

enum { max_length = 1024 };

peer_info client::registerer(register_request register_request) {
  try {
    udp::endpoint register_endpoint(address::from_string(REGISTERER_SERVER_IP),
                                    REGISTERER_SERVER_PORT);
    boost::system::error_code error;
    auto serialized = register_request.serialize();

    packet register_request(PSH, serialized);
    auto data = register_request.serialize();

    socket.send_to(boost::asio::buffer(data.first, data.second),
                   register_endpoint);

    auto ack_res = receive_from(register_endpoint, error);

    if (ack_res.get_type() != ACK)
      throw std::runtime_error("Couldn't check in with server.");

    std::cout << "Checked in with server. Waiting for peer to connect..."
              << std::endl;

    auto peer_data = receive_from(register_endpoint, error);

    return peer_info::deserilize(peer_data.get_content());
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return {};
  }
}

packet client::receive_from(udp::endpoint remote_endpoint,
                             boost::system::error_code& error) {
  char buffer[max_length];
  auto length = socket.receive_from(boost::asio::buffer(buffer, max_length),
                                    remote_endpoint, 0, error);

  packet data;
  data.deserialize(buffer, length);

  return data;
}