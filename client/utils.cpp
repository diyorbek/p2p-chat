#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>

#include "utils.h"

int utils::get_terminal_char_width() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

void utils::print_received_message(peer_info& peer, std::string& message) {
  std::string cleaner(utils::get_terminal_char_width(), ' ');
  std::cout << '\r' << cleaner;
  std::cout << '\r' << (peer.name + ": " + message) << '\n';
  std::cout << "> " << std::flush;
}

std::string utils::input_message() {
  std::string message;
  std::cout << "\r> ";
  std::getline(std::cin, message);
  message += '\n';
  return message;
}

void utils::print_sent_message(std::string& message) {
  std::cout << "\033[F\r> " << message << std::flush;
}

utils::cmd_options utils::get_cmd_options(int argc, char* argv[]) {
  cmd_options options{};

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0) {
      options.is_debug_mode = true;
      continue;
    }

    // get source port
    if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      options.source_port = std::stoi(argv[++i]);
      continue;
    }

    // get peer name and address to connect to
    if (strcmp(argv[i], "-c") == 0 && i + 3 < argc) {
      options.known_peer.name = argv[++i];
      options.known_peer.ip = argv[++i];
      options.known_peer.port = argv[++i];

      options.has_known_peer =
          !(options.known_peer.name.empty() || options.known_peer.ip.empty() ||
            options.known_peer.port.empty());

      continue;
    }
  }

  return options;
}
