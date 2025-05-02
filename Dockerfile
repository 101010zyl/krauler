FROM ubuntu:22.04

# Install compilers and libs
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libcurl4-openssl-dev \
    libgumbo-dev \
    libsqlite3-dev \
    clang-format \
    clang-tidy \
    git \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN useradd -ms /bin/bash devuser
USER devuser
WORKDIR /home/devuser/krauler

CMD ["bash"]
