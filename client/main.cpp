#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>

#include "client.h"
#include "http_server.h"
#include "registerer.h"
#include "utils.h"
#include "ws_server.h"

using namespace std;

void start_ui_mode(utils::cmd_options& options) {
  try {
    boost::asio::io_service io_service;
    client client(io_service);
    register_request data;
    start_http_server(data);

    auto peer = client.registerer(data);
  } catch (exception& e) {
    cerr << "Exception in start_ui_mode: " << e.what() << "\n";
  }
}

void start_cli_mode(utils::cmd_options& options) {
  try {
    boost::asio::io_service io_service;
    client client(io_service);

    if (options.has_known_peer) {
      client.connect(options.known_peer);
      return;
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
    cerr << "Exception in start_cli_mode: " << e.what() << "\n";
  }
}

int main(int argc, char* argv[]) {
  try {
    auto options = utils::get_cmd_options(argc, argv);
    boost::asio::io_service io_service;
    client client(io_service, options.source_port);

    start_ui_mode(options);
    if (options.is_ui_mode) {
      return 0;
    }

    start_cli_mode(options);
  } catch (exception& e) {
    cerr << "Exception in main: " << e.what() << "\n";
  }

  return 0;
}