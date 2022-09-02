#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>

#include "./client.h"
#include "./registerer.h"

using namespace std;

int main(int argc, char* argv[]) {
  bool is_debug_mode = argc > 1 && strcmp(argv[1], "-d") == 0;

  try {
    boost::asio::io_service io_service;
    client client(io_service);
    register_request request;

    cout << "Enter your name: ";
    cin >> request.name;
    cout << "Enter room name: ";
    cin >> request.room_name;

    auto peer = client.registerer(request);

    cout << "\nPeer found: " << peer.raw << flush << "\n\n";

    if (!is_debug_mode)
      client.connect(peer);
  } catch (exception& e) {
    cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}