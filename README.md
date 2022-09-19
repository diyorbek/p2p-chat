# Peer to peer chat app

A CLI chat application based on p2p connection. The peers eshtablish direct connection and the messages are sent only between them using UDP protocol.

The communication is done via the [UDP Hole Punching](https://en.wikipedia.org/wiki/UDP_hole_punching) technique. There is a rendezvous-server which helps peers to exchange their IPs and port numbers. After matching the peers, the server does not participate in the messaging between the peers. The messages are serialized but not encrypted.

## Setup

There is a docker configuration for the rendezvous-server for Ubuntu. If you're using Docker then just run this command in `./renderzvous-server` folder:

```bash
docker-compose up
```

It installs all the dependencies automatically.

The same configuration can be using for the client application. To manually setup the project, install these dependencies:

**CMake** - [Installation guide](https://cmake.org/install/).

**Boost C++** - Project uses Boost Asio as it has rich API for network communication. Here are the instructions for [installing Boost on Windows, Ubuntu](https://robots.uc3m.es/installation-guides/install-boost.html) and [MacOS](https://stackoverflow.com/a/9196677/9838291).

## Build

After installing all the dependencies go to the application folder you want to build and run these commands:

```bash

cd rendezvous-server # if you want to build the server
cd client # if you want to build the client

mkdir ./build
cmake -B/build -S . -D CMAKE_BUILD_TYPE=Release
cmake --build /build
```

## Needs improvement

- Message encryption.
- Send `KEEP_ALIVE` packets periodically to prevent UDP Hole from closing.
  - Aternatively: Punch a new UDP hole in a specific time interval.
