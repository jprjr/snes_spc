FROM gcc:9

RUN apt-get update && \
    apt-get install -y curl gpg

RUN echo "deb http://apt.llvm.org/buster/ llvm-toolchain-buster-9 main" > /etc/apt/sources.list.d/llvm.list && \
    echo "deb-src http://apt.llvm.org/buster/ llvm-toolchain-buster-9 main" >> /etc/apt/sources.list.d/llvm.list && \
    curl -SsL https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
      clang-9 \
      llvm-9 \
      lld-9 \
      clang-tools-9 \
      perl \
      libfindbin-libs-perl \
      make

COPY . /src/snes_spc
RUN cd /src/snes_spc \
    && make clean && make -j$(nproc) CC=clang-9 CXX=clang++-9 \
    && make clean && make -j$(nproc) CC=gcc CXX=g++ \
    && make install

FROM gcc:8
COPY . /src/snes_spc
RUN cd /src/snes_spc \
    && make clean && make -j$(nproc) CC=gcc CXX=g++ \
    && make install

FROM gcc:7
COPY . /src/snes_spc
RUN cd /src/snes_spc \
    && make clean && make -j$(nproc) CC=gcc CXX=g++ \
    && make install

FROM gcc:6
COPY . /src/snes_spc
RUN cd /src/snes_spc \
    && make clean && make -j$(nproc) CC=gcc CXX=g++ \
    && make install

FROM gcc:5
COPY . /src/snes_spc
RUN cd /src/snes_spc \
    && make clean && make -j$(nproc) CC=gcc CXX=g++ \
    && make install

FROM gcc:4
COPY . /src/snes_spc
RUN cd /src/snes_spc \
    && make clean && make -j$(nproc) CC=gcc CXX=g++ \
    && make install
