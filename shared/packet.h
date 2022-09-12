#pragma once

#include <iostream>

enum packet_type { ACK = 100, PSH };

struct packet {
  static u_short __seq_num;

 private:
  u_short _seq_num;
  packet_type _type;
  std::string _content;

 public:
  packet();
  packet(packet_type type);
  packet(packet_type type, std::string str);

  u_short get_seq_num();
  packet_type get_type();
  std::string get_content();
  u_short get_content_length();

  void put_content(std::string str);

  std::pair<char*, size_t> serialize() const;

  void deserialize(char* serialized, size_t size);
};
