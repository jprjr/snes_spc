FROM alpine:latest

RUN apk add --no-cache gcc g++ musl-dev make perl

COPY . /src/snes_spc
RUN cd /src/snes_spc && make clean && make
