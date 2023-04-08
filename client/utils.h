#pragma once

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>

#include "peer_info.h"

namespace utils {

int get_terminal_char_width();

void print_received_message(peer_info& peer, std::string& message);

std::string input_message();

void print_sent_message(std::string& message);

struct cmd_options {
  bool is_debug_mode;
  bool is_ui_mode;
  u_short source_port;
  bool has_known_peer;
  peer_info known_peer;
};

cmd_options get_cmd_options(int argc, char* argv[]);

}  // namespace utils