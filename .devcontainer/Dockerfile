FROM gcc:13

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       cmake ninja-build git gdb ca-certificates \
    && rm -rf /var/lib/apt/lists/*
