#!/bin/bash

g++ -pthread -lstdc++fs sort.cpp -o sort
./sort $1 $2
rm sort