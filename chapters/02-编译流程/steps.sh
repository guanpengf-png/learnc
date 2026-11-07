#!/bin/sh
# 把预处理、编译、汇编、链接四步拆开，中间文件会留在当前目录。
# 用法：在本目录执行  sh steps.sh
cd "$(dirname "$0")" || exit 1

cc -std=c11 -E main.c -o main.i
cc -std=c11 -S main.i -o main.s
cc -std=c11 -c main.s -o main.o
cc main.o -o main
./main
