#!/usr/bin/env bash
rm -rf coverage
mkdir coverage
cp satori.cxx coverage/
cd coverage
g++ -Wall -Wextra -fprofile-arcs -ftest-coverage -coverage satori.cxx -o satori
./satori < ../testy/t$1.in > ../testy/t$1.tout
lcov --directory . --capture --output-file app.info
genhtml app.info
konqueror index.html
