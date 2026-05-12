FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential cmake g++ valgrind git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

CMD ["./build/run_tests"]