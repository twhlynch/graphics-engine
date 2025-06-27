#!/bin/bash

# ./build.sh [args...] <CMake Generator>

args=("$@")

config="Debug"
generator=""
generate_docs="OFF"
optimise="OFF"
run=false
help=false

flags=("-R" "-d" "-r" "-o" "-h")
long_flags=("--release" "--docs" "--run" "--optimise" "--help")

check_flag() {
	local arg="$1"
	local index="$2"

	[[ "$arg" == ${flags[index]} || "$arg" == ${long_flags[index]} ]]
}

for arg in "${args[@]}"; do
	if   check_flag "$arg" 0; then config="Release"
	elif check_flag "$arg" 1; then generate_docs="ON"
	elif check_flag "$arg" 2; then run=true
	elif check_flag "$arg" 3; then optimise="ON"
	elif check_flag "$arg" 4; then help=true
	fi
done

if [[ $help == true ]]; then
	echo "  usage: ./build.sh [options] [\"CMake Generator\"]"
	echo "         Build the project with a specified generator (or 'default')"
	echo "options:"
	echo "         -R --release     Build Release target"
	echo "         -d --docs        Build Doxygen documentation"
	echo "         -r --run         Run after build"
	echo "         -o --optimise    Adds the -O3 compile option"
	echo "         -h --help        Show this message"

	exit 0
fi

filtered_args=()
for arg in "${args[@]}"; do
	filter=false
	for ((i = 0; i < ${#flags[@]}; i++)); do
		if check_flag "$arg" "$i"; then
			filter=true
		fi
	done
	[[ $filter == false ]] && filtered_args+=("$arg")
done

if [[ ${#filtered_args[@]} -gt 0 ]]; then
	generator="${filtered_args[0]}"
fi

folder_name="${generator// /_}"
[ -z "$folder_name" ] && folder_name="default"

mkdir -p "build/$folder_name"
cd "build/$folder_name"

if [[ -n "$generator" ]]; then
	cmake -G "$generator" -DGENERATE_DOCUMENTATION="$generate_docs" -DOPTIMISE="$optimise" ../..
else
	cmake -DGENERATE_DOCUMENTATION="$generate_docs" -DOPTIMISE="$optimise" ../..
fi

cmake --build . --config "$config"

if [[ $run == true ]]; then
	executable="Engine"
	if [[ -d "$config" ]]; then
		executable="$config/Engine"
	fi

	./$executable
fi
