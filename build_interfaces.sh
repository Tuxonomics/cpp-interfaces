#!/bin/bash

clang++ -o interfaces_all -std=c++11 -O3 interfaces_all.cpp

time ./interfaces_all
