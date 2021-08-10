sudo apt-get install \
    g++ \
    cmake \
    libboost-all-dev \
    libevent-dev \
    libdouble-conversion-dev \
    libgoogle-glog-dev \
    libgflags-dev \
    libiberty-dev \
    liblz4-dev \
    liblzma-dev \
    libsnappy-dev \
    make \
    zlib1g-dev \
    binutils-dev \
    libjemalloc-dev \
    libssl-dev \
    pkg-config \
    libunwind-dev

sudo apt-get install \
    libunwind8-dev \
    libelf-dev \
    libdwarf-dev

wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz && \
    tar zxf release-1.8.0.tar.gz && \
    rm -f release-1.8.0.tar.gz && \
    cd googletest-release-1.8.0 && \
    cmake . && \
    make && \
    sudo make install
cd ../..

git clone https://github.com/fmtlib/fmt.git && cd fmt
mkdir _build && cd _build
cmake ..
make -j$(nproc)
sudo make install
cd ../..

# git clone https://github.com/google/double-conversion.git
# cd double-conversion
# cmake -DBUILD_SHARED_LIBS=ON .
# make
# sudo make install
# cd ../..

git clone https://github.com/facebook/folly.git
cd folly
mkdir build && build
cmake -DBUILD_TESTS=ON ..
make -j$(nproc)
sudo make install
cd ../..
