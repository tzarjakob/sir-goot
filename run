#! /bin/bash

# Use the option -v to run valgrind analyzer
valg="-v"
debug="-d"

if [[ $1 == $valg ]]
then valgrind ./build/Game
elif [[ $1 == $debug ]]
then gdb ./build/Game
else
./build/Game
fi 