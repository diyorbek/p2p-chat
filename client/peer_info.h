#pragma once

#include <boost/algorithm/string.hpp>
#include <sstream>
#include <vector>

struct peer_info {
  std::string name;
  std::string ip;
  std::string port;
  std::string raw;

  static peer_info deserilize(std::string raw_data) {
    boost::trim(raw_data);
    std::vector<std::string> data;
    boost::split(data, raw_data, boost::is_any_of(" "));

    peer_info peer;
    peer.name = data[0];
    peer.ip = data[1];
    peer.port = data[2];
    peer.raw = raw_data;

    return peer;
  }
};