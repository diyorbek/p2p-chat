#include <iostream>

#include "packet.h"

u_short packet::__seq_num = 0;

packet::packet() {}

packet::packet(packet_type type) : _type(type), _seq_num(++__seq_num) {}

packet::packet(packet_type type, std::string str) : packet(type) {
  put_content(str);
}

u_short packet::get_seq_num() {
  return _seq_num;
}

packet_type packet::get_type() {
  return _type;
}

std::string packet::get_content() {
  return _content;
}

u_short packet::get_content_length() {
  return _content.length();
}

void packet::put_content(std::string str) {
  _content = str;
}

std::pair<char*, size_t> packet::serialize() const {
  // calculate size including the actual length of the content
  u_short _length = _content.length();
  size_t size = sizeof(_type) + sizeof(_length) + _length + 1;
  char* serialized = new char[size];
  auto cursor = serialized;

  memcpy(cursor, &_type, sizeof(_type));
  cursor += sizeof(_type);
  memcpy(cursor, &_length, sizeof(_length));
  cursor += sizeof(_length);
  memcpy(cursor, _content.c_str(), _length);

  return {serialized, size};
}

void packet::deserialize(char* serialized, size_t size) {
  auto cursor = serialized;

  memcpy(&_type, cursor, sizeof(_type));
  cursor += sizeof(_type);

  u_short _length;
  memcpy(&_length, cursor, sizeof(_length));
  cursor += sizeof(_length);

  // allocate memory according to length field
  _content = std::string(cursor, _length);
}
