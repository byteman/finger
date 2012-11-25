#! /bin/sh

protoc --cpp_out=../src  finger.proto
protoc --cpp_out=../test finger.proto
