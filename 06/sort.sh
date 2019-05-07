#!/bin/bash

g++-8 sort.cpp -o sort -std=c++17 -pthread -lstdc++fs
./sort $1 $2
rm sort
