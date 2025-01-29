FROM ubuntu:noble

RUN apt update && apt install -y g++

WORKDIR /code
