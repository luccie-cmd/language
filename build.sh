#!/bin/bash

set -e

CC=cc
CFLAGS="-Wall -Wextra -pedantic -fsanitize=address -std=c11 -Iinclude"

build_dir() {
    mkdir -p out
    echo ${CC} ${CFLAGS} -o ./out/main ./src/*.c
    ${CC} ${CFLAGS} -o ./out/main ./src/*.c
}

build_dir