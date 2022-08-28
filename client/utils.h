#pragma once

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>

namespace utils {

int get_terminal_char_width() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

void print_received_message(peer_info& peer, std::string& message) {
  std::string cleaner(utils::get_terminal_char_width(), ' ');
  std::cout << '\r' << cleaner;
  std::cout << '\r' << (peer.name + ": " + message) << '\n';
  std::cout << "> " << std::flush;
}

std::string input_message() {
  std::string message;
  std::cout << "\r> ";
  std::getline(std::cin, message);
  message += '\n';
  return message;
}

void print_sent_message(std::string& message) {
  std::cout << "\033[F\r> " << message << std::flush;
}

}  // namespace utils