#include <iostream>

#include "request.h"

u_short request::__seq_num = 0;

request::request() {}

request::request(request_type type) : _type(type), _seq_num(++__seq_num) {}

request::request(request_type type, std::string str) : request(type) {
  put_content(str);
}

u_short request::get_seq_num() {
  return _seq_num;
}

char* request::get_content() {
  return _content;
}

u_short request::get_content_length() {
  return _length;
}

void request::put_content(std::string str) {
  _length = str.length();
  memcpy(_content, str.c_str(), str.length());
}

std::pair<char*, size_t> request::serialize() const {
  // calculate size including the actual length of the content
  size_t size = sizeof(_type) + sizeof(_length) + _length + 2;
  char* serialized = new char[size];
  auto cursor = serialized;

  memcpy(cursor, &_type, sizeof(_type));
  cursor += sizeof(_type);
  memcpy(cursor, &_length, sizeof(_length));
  cursor += sizeof(_length);
  memcpy(cursor, _content, _length + 1);

  return {serialized, size};
}

void request::deserialize(char* serialized, size_t size) {
  auto cursor = serialized;

  memcpy(&_type, cursor, sizeof(_type));
  cursor += sizeof(_type);
  memcpy(&_length, cursor, sizeof(_length));
  cursor += sizeof(_length);

  // allocate memory according to length field
  _content = new char[_length + 1];
  memcpy(_content, cursor, _length + 1);
}

std::ostream& request::serialize(std::ostream& os) const {
  char null = '\0';

  os.write((char*)&_type, sizeof(_type));
  os.write((char*)&_length, sizeof(_length));
  os.write(_content, strlen(_content));
  os.write(&null, 1);

  return os;
}

std::istream& request::deserialize(std::istream& is) {
  is.read((char*)&_type, sizeof(_type));
  is.read((char*)&_length, sizeof(_length));

  int i = 0;
  _content = new char[_length + 1];

  while (!is.eof()) {
    _content[i++] = is.get();
  }

  return is;
}
