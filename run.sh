#!/bin/bash

help_msg() {
    [[ -n $1 ]] && echo "ERROR: $1"
    echo "Usage: $0 [OPTION]
--lwe       run lwe test
--tlwe      run tlwe test"
}

# Argument parsing
if [ $# -eq 0 ]; then
    help_msg "Too few arguments. See below for reference"
    exit 1
else
    while [[ $# -gt 0 ]]; do
        case $1 in
            --lwe)
            LWE=1
            shift
            ;;
            --tlwe)
            TLWE=1
            shift
            ;;
            *)
            help_msg "Invalid argument. See below for reference"
            exit 1
            ;;
        esac
    done
fi

if [ "$LWE" != "" ] && [ $LWE -eq 1 ]; then
    outputFile="lwetest.o"
    g++ -g -o $outputFile src/lwetest.cpp src/lwe/* src/general/*
elif [ "$TLWE" != "" ] && [ $TLWE -eq 1 ]; then
    outputFile="tlwetest.o"
    g++ -g -o $outputFile src/tlwetest.cpp src/lwe/* src/tlwe/* src/general/*
fi

./$outputFile
exit 0
