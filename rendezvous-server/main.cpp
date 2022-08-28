#include <boost/algorithm/string.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <condition_variable>
#include <cstdlib>
#include <iostream>
#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

using boost::asio::ip::udp;
using namespace std;

const int max_length = 1024;

struct peer {
  udp::endpoint remote_endpoint;
  string ip;
  u_short port;
  string name;
  string room_name;
};

typedef unordered_map<string, peer> room_hosts;
typedef map<pair<string, u_short>, string> Sessions;
room_hosts hosts;
Sessions sessions;

peer parse_request(udp::endpoint& remote_endpoint, string& raw_request) {
  auto address = remote_endpoint.address().to_string();
  auto port = remote_endpoint.port();
  boost::trim(raw_request);
  vector<string> request;
  boost::split(request, raw_request, boost::is_any_of(" "));

  if (request.size() != 2) {
    throw std::runtime_error(to_string(port) + " sent invalid request\n");
  }

  peer peer;
  peer.remote_endpoint = remote_endpoint;
  peer.ip = address;
  peer.port = port;
  peer.name = request[0];
  peer.room_name = request[1];

  return peer;
}

string serialize_peer_info(peer peer) {
  return peer.name + ' ' + peer.ip + ' ' + to_string(peer.port) + '\n';
}

void handle_request(udp::socket& socket,
                    udp::endpoint remote_endpoint,
                    string raw_request) {
  auto address = remote_endpoint.address().to_string();
  auto port = remote_endpoint.port();

  if (sessions.count({address, port})) {
    return;
  }

  peer new_peer = parse_request(remote_endpoint, raw_request);
  auto room_name = new_peer.room_name;

  sessions.insert({{address, port}, room_name});

  if (hosts.count(room_name) == 0) {
    hosts.emplace(room_name, new_peer);

    return;
  }

  auto peer1 = hosts[room_name];
  auto peer2 = new_peer;
  auto peer1_data = serialize_peer_info(peer1);
  auto peer2_data = serialize_peer_info(peer2);

  socket.send_to(boost::asio::buffer(peer2_data), peer1.remote_endpoint);
  socket.send_to(boost::asio::buffer(peer1_data), peer2.remote_endpoint);

  sessions.erase({peer1.ip, peer1.port});
  sessions.erase({peer2.ip, peer2.port});
  hosts.erase(room_name);
}

void server(boost::asio::io_service& io_service, short port) {
  udp::socket socket(io_service, udp::endpoint(udp::v4(), port));

  while (true) {
    char data[max_length];
    udp::endpoint remote_endpoint;
    boost::system::error_code error;

    size_t length = socket.receive_from(boost::asio::buffer(data),
                                        remote_endpoint, 0, error);

    if (error && error != boost::asio::error::message_size)
      std::cerr << "Exception: " << error.what() << "\n";

    handle_request(socket, remote_endpoint, string(data, length));
  }
}

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_service io_service;

    server(io_service, 8081);
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}