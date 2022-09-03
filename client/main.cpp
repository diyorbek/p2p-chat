#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>

#include "client.h"
#include "registerer.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]) {
  try {
    auto options = utils::get_cmd_options(argc, argv);
    boost::asio::io_service io_service;
    client client(io_service, options.source_port);

    if (options.has_known_peer) {
      client.connect(options.known_peer);
      return 0;
    }

    register_request request;

    cout << "Enter your name: ";
    cin >> request.name;
    cout << "Enter room name: ";
    cin >> request.room_name;

    auto peer = client.registerer(request);

    cout << "\nPeer found: " << peer.raw << flush << "\n\n";

    if (!options.is_debug_mode)
      client.connect(peer);
  } catch (exception& e) {
    cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}