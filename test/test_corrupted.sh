#!/bin/bash
# Ansi color code variables
red="\e[0;91m"
blue="\e[0;94m"
expand_bg="\e[K"
blue_bg="\e[0;104m${expand_bg}"
red_bg="\e[0;101m${expand_bg}"
green_bg="\e[0;102m${expand_bg}"
green="\e[0;92m"
white="\e[0;97m"
bold="\e[1m"
uline="\e[4m"
reset="\e[0m"

##### PATH #####
curr_dir=$PWD
dir=${curr_dir##*/}

if [ $dir = 'test' -o $dir = 'libft' -o $dir = 'srcs' ]; then
	cd ..
elif [ $dir = 'woody_woodpacker' ]; then
	cd .
else
	echo "you are'nt in a good directory, go to woody_woodpacker/";
	exit;
fi

if [ ! -d test/log ]; then mkdir test/log; fi

BIN="test/corrupted_bin/"
corrupted_bin=($BIN*.out)
LOG="test/log/"

print_help () {
	echo "usage: bash test_corrupted.sh

  --help  prints this help";
}

rm -f $LOG*.txt

run_comparison () {
	local test_nb=0
	for file in "$@"; do
		printf "\n[$test_nb] - $bold$white$file:$reset\n"
		rm -rf woody
		printf "$red""PACKING: $reset"
		./woody_woodpacker $file
		printf "$green""run file: $reset"
		./$file
		printf "$green""run woody: $reset"
		./woody
		((test_nb=test_nb+1));
	done
}

if [ "$1" == "--help" ]; then
	print_help;
else run_comparison ${corrupted_bin[@]};
fi
