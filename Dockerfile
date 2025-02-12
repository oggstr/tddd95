FROM ubuntu:noble

RUN apt update && apt install -y g++ gcc-14 g++-14

WORKDIR /code
