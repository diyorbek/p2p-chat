#pragma once

#include <boost/asio.hpp>
#include <string>

#include "../shared/request.h"
#include "peer_info.h"
#include "registerer.h"

using boost::asio::ip::udp;

class client {
  enum { max_length = 1024 };
  boost::asio::io_service& io_svc;
  udp::socket socket;
  peer_info* connected_peer;

 public:
  client(boost::asio::io_service& io_service);
  client(boost::asio::io_service& io_service, u_short port);

  peer_info registerer(register_request request);
  void connect(peer_info& peer);

 private:
  request receive_from(udp::endpoint remote_endpoint,
                       boost::system::error_code& error);

  std::string input_message();
  std::string receive_message();
  void start_message_receive();
  void start_message_send();
};