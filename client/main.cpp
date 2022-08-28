#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>

#include "./client.h"
#include "./registerer.h"

using namespace std;

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_service io_service;
    client client(io_service);
    register_request request;

    cout << "Enter your name: ";
    cin >> request.name;
    cout << "Enter room name: ";
    cin >> request.room_name;

    auto peer = client.registerer(request);

    client.connect(peer);
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}