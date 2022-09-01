#pragma once

#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>

using boost::asio::ip::udp;

struct peer {
  udp::endpoint remote_endpoint;
  std::string ip;
  u_short port;
  std::string name;
  std::string room_name;
  std::string raw;

  std::string serialize() {
    return name + ' ' + ip + ' ' + std::to_string(port) + '\n';
  }

  static peer deserilize(udp::endpoint& remote_endpoint, std::string raw_data) {
    auto address = remote_endpoint.address().to_string();
    auto port = remote_endpoint.port();

    boost::trim(raw_data);
    std::vector<std::string> data;
    boost::split(data, raw_data, boost::is_any_of(" "));

    if (data.size() != 2)
      throw std::runtime_error(std::to_string(port) + " sent invalid data");

    peer peer;
    peer.remote_endpoint = remote_endpoint;
    peer.ip = address;
    peer.port = port;
    peer.name = data[0];
    peer.room_name = data[1];

    return peer;
  }
};
