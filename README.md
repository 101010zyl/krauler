# Krauler
TODO: docker
## Dependencies
```
sudo apt install -y build-essential \
    cmake \
    llvm \
    libcurl4-openssl-dev \
    libgumbo-dev \
    libsqlite3-dev \
    clang-format \
    clang-tidy \
    git \
    libboost-all-dev \
    libssl-dev \
    clangd \
    libspdlog-dev \
    libjsoncpp-dev 
```
## Build
```
git clone https://github.com/101010zyl/krauler
cd krauler
```
```
rm -rf build
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
## Run
```
./build/krauler -u https://www.example.com
```