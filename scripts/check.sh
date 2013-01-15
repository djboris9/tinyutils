#!/bin/bash


gcc -Wall -g -pg b64.c -o b64.out
cat testfile.dat | ./b64 > /dev/null
echo "-------------------"
echo "Profile"
echo "-------------------"
gprof -b b64.out


gcc -Wall -O3 b64.c -o b64.out


echo "-------------------"
echo "b64"
echo "-------------------"
time cat testfile.dat | ./b64.out > /dev/null

echo "-------------------"
echo "base64"
echo "-------------------"
time cat testfile.dat | base64 > /dev/null

cat testfile.dat | ./b64.out > /tmp/b64
cat testfile.dat | base64 > /tmp/base64
diff -u /tmp/b64 /tmp/base64
rm /tmp/b64 /tmp/base64

# Leakcheck:
valgrind -v --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./b64.out
