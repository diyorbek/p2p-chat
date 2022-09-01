
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <vector>

#include "../shared/request.h"
#include "client.h"
#include "peer_info.h"
#include "registerer.h"

#define REGISTERER_SERVER_IP "0.0.0.0"  // "46.101.118.227"
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

    request request;
    request.type = PSH;
    request.put_content(serialized);
    auto data = request.serialize();

    socket.send_to(boost::asio::buffer(data.first, data.second),
                   register_endpoint);

    std::cout << "Checked in with server. Waiting for peer to connect..."
              << std::endl;

    char streambuf[1024];
    socket.receive_from(boost::asio::buffer(streambuf), register_endpoint, 0,
                        error);

    std::ostringstream ss;
    ss << streambuf;

    return peer_info::deserilize(ss.str());
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return {};
  }
}