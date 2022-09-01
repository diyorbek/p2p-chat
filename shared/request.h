#pragma once

#include <iostream>

enum request_type { ACK, PSH };

struct request {
  request_type type;
  u_short length;
  char* content;

  void put_content(std::string str) {
    length = str.length();
    memcpy(content, str.c_str(), str.length());
  }

  std::pair<char*, size_t> serialize() const {
    // calculate size including the actual length of the content
    size_t size = sizeof(type) + sizeof(length) + length + 2;
    char* serialized = new char[size];
    auto cursor = serialized;

    memcpy(cursor, &type, sizeof(type));
    cursor += sizeof(type);
    memcpy(cursor, &length, sizeof(length));
    cursor += sizeof(length);
    memcpy(cursor, content, length + 1);

    return {serialized, size};
  }

  void deserialize(char* serialized, size_t size) {
    auto cursor = serialized;

    memcpy(&type, cursor, sizeof(type));
    cursor += sizeof(type);
    memcpy(&length, cursor, sizeof(length));
    cursor += sizeof(length);

    // allocate memory according to length field
    content = new char[length + 1];
    memcpy(content, cursor, length + 1);
  }

  std::ostream& serialize(std::ostream& os) const {
    char null = '\0';

    os.write((char*)&type, sizeof(type));
    os.write((char*)&length, sizeof(length));
    os.write(content, strlen(content));
    os.write(&null, 1);

    return os;
  }

  std::istream& deserialize(std::istream& is) {
    is.read((char*)&type, sizeof(type));
    is.read((char*)&length, sizeof(length));

    int i = 0;
    content = new char[length + 1];

    while (!is.eof()) {
      content[i++] = is.get();
    }

    return is;
  }
};