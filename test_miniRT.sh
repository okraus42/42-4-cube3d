#!/usr/bin/bash

if [[ $# -eq 2 ]]; then
	if [[ $2 == "--valgrind" ]]; then
		valgrind -s --leak-check=full --show-reachable=yes --error-limit=no --suppressions=file.supp ./miniRT $1;
	elif [[ $1 == "--valgrind" ]]; then
		valgrind -s --leak-check=full --show-reachable=yes --error-limit=no --suppressions=file.supp ./miniRT $2;
	fi
elif [[ $# -eq 1 ]]; then
	./miniRT $1;
else
	echo "Provide parameters: [--valgrind;pathToFile]";
fi
