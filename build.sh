#!/bin/bash

# ./build.sh [Generator] [Config]

generator=""
config="Debug"

if [[ "$1" == "Release" ]]; then
	config="Release"
elif [[ -n "$1" ]]; then
	generator="$1"
	if [[ "$2" == "Release" ]]; then
		config="Release"
	fi
fi

folder_name="${generator// /_}"
[ -z "$folder_name" ] && folder_name="default"

mkdir -p "build/$folder_name"
cd "build/$folder_name"

if [[ -n "$generator" ]]; then
	cmake -G "$generator" ../..
else
	cmake ../..
fi

cmake --build . --config "$config"