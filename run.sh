#!/usr/bin/env bash
set -euo pipefail
cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
./build/ip_filter < ip_filter.tsv > result.txt 2> search.txt
cat search.txt
printf '\nFirst 15 lines of result.txt:\n'
head -n 15 result.txt
