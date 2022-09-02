#pragma once

#include <iostream>

enum request_type { ACK = 100, PSH };

struct request {
  static u_short __seq_num;

 private:
  u_short _seq_num;
  request_type _type;
  u_short _length;
  char* _content;

 public:
  request();
  request(request_type type);
  request(request_type type, std::string str);

  u_short get_seq_num();
  char* get_content();
  u_short get_content_length();

  void put_content(std::string str);

  std::pair<char*, size_t> serialize() const;
  std::ostream& serialize(std::ostream& os) const;

  void deserialize(char* serialized, size_t size);
  std::istream& deserialize(std::istream& is);
};
