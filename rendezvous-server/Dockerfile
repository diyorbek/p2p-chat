FROM ubuntu:latest
ARG DEBIAN_FRONTEND=noninteractive

# Install required build dependencies
RUN apt-get -y update && apt-get install -y
RUN apt-get -y install g++ cmake git
RUN apt-get -y --no-install-recommends install \
  build-essential \
  gdb \
  wget

# Install boost
RUN cd ${HOME} && \
  wget --no-check-certificate --quiet \
  https://boostorg.jfrog.io/artifactory/main/release/1.77.0/source/boost_1_77_0.tar.gz && \
  tar xzf ./boost_1_77_0.tar.gz && \
  cd ./boost_1_77_0 && \
  ./bootstrap.sh && \
  ./b2 install --with-thread --with-system --with-regex && \
  cd .. && \
  rm -rf ./boost_1_77_0

COPY . .
WORKDIR .

# Run cmake configure & build process
RUN mkdir ./build
RUN cmake -B/build -S . -D CMAKE_BUILD_TYPE=Release
RUN cmake --build /build

# Launch built application
CMD ["./build/chat-server"]