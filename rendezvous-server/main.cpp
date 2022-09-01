#include <boost/asio.hpp>
#include <iostream>

#include "peer.h"
#include "server.h"

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_service io_service;
    server(io_service, 8081).start();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}