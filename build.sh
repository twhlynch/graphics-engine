#!/bin/bash

# ./build.sh [args...] (Release, Docs, <CMake Generator>)

args=("$@")
config="Debug"
generate_docs="OFF"
generator=""

for arg in "${args[@]}"; do
	if [[ "$arg" == "Release" ]]; then
		config="Release"
	elif [[ "$arg" == "Docs" ]]; then
		generate_docs="ON"
	fi
done

filtered_args=()
for arg in "${args[@]}"; do
	if [[ "$arg" != "Release" && "$arg" != "Docs" ]]; then
		filtered_args+=("$arg")
	fi
done

if [[ ${#filtered_args[@]} -gt 0 ]]; then
	generator="${filtered_args[0]}"
fi

folder_name="${generator// /_}"
[ -z "$folder_name" ] && folder_name="default"

mkdir -p "build/$folder_name"
cd "build/$folder_name"

if [[ -n "$generator" ]]; then
	cmake -G "$generator" -DGENERATE_DOCUMENTATION="$generate_docs" ../..
else
	cmake -DGENERATE_DOCUMENTATION="$generate_docs" ../..
fi

cmake --build . --config "$config"
