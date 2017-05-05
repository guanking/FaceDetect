#!/bin/sh
src=$(ls | grep .cpp$);
libs=$(pkg-config opencv --libs);
g++ ${src} ${libs} -o main.exe;
