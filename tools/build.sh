#!/usr/bin/env bash
set -e
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cmake -B "${ROOT_DIR}/build"
cmake --build "${ROOT_DIR}/build" --target cmdcontroller --target shmclean --target fproxy -j"$(nproc 2>/dev/null || echo 4)"
