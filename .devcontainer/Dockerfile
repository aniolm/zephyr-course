FROM ghcr.io/zephyrproject-rtos/ci:main

# Optional: install extras
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    ninja-build \
    gperf \
    ccache \
    dfu-util \
    device-tree-compiler \
    wget \
    python3-pip \
    && apt-get clean