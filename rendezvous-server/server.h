#include <boost/asio.hpp>
#include <map>
#include <unordered_map>
#include <vector>

#include "../shared/request.h"

using boost::asio::ip::udp;

typedef std::unordered_map<std::string, peer> room_hosts;

// using `map`instead of `unordered_map` because it supports `std::pair` as key
typedef std::map<std::pair<std::string, u_short>, std::string> Sessions;

struct received {
  udp::endpoint remote_endpoint;
  request request;
};

class server {
  udp::socket socket;
  room_hosts hosts;
  Sessions sessions;
  int max_buffer_size = 1024;

 public:
  server(boost::asio::io_service& io_service, short port);

  void start();
  received receive();
  void handle_request(received received);
};