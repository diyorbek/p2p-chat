#pragma once

#include <sstream>
#include <vector>

#include "peer_info.h"

struct register_request {
  std::string name;
  std::string room_name;

  std::string serialize() {
    std::stringstream ss;
    ss << name << " " << room_name << '\n';

    return ss.str();
  }
};