FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y clang build-essential && \
    apt-get install -y cmake && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN mkdir build && cd build && \
    cmake .. && \
    cmake --build .

WORKDIR /app/build

ENV SAMPLE_NAME="sample.txt"

CMD ["sh", "-c", "./sw_battle_test ../test/data/${SAMPLE_NAME}"]

#CMD ["/bin/bash"]