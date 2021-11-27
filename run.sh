#!/bin/bash

help_msg() {
    [[ -n $1 ]] && echo "ERROR: $1"
    echo "$0 usage:" && grep ".)\ *#" $0; exit 0;
}

# Argument parsing
if [ $# -eq 0 ]; then
    help_msg "Too few arguments. See below for reference"
    exit 1
else
    while [[ $# -gt 0 ]]; do
        case $1 in
            --lwe)          # run lwe test
            LWE=1
            shift
            ;;
            --tlwe)         # run tlwe test
            TLWE=1
            shift
            ;;
            --tgsw)         # run tgsw test
            TGSW=1
            shift
            ;;
            --gate)         # test gateKey creation
            GATE=1
            shift
            ;;
            --extprod)      # test external product function
            EXTPROD=1
            shift
            ;;
            --cmux)         # Is CMUX working?
            CMUX=1
            shift
            ;;
            --blindrot)     # Can I rotate?
            BLINDROTATION=1
            shift
            ;;
            --gentestvect)     # Can I rotate?
            gentestvect=1
            shift
            ;;
            --boot)         # Let's check bootstrapping function
            BOOTSTRAPPING=1
            shift
            ;;
            --all)          # Execute all tests and save output in tests/ dir
            ALL=1
            shift
            ;;
            --help | *)     # Show help
            help_msg "Invalid argument. See below for reference"
            exit 1
            ;;
        esac
    done
fi

if [ "$LWE" != "" ] && [ $LWE -eq 1 ]; then
    outputFile="lwetest"
elif [ "$TLWE" != "" ] && [ $TLWE -eq 1 ]; then
    outputFile="tlwetest"
elif [ "$TGSW" != "" ] && [ $TGSW -eq 1 ]; then
    outputFile="tgswtest"
elif [ "$GATE" != "" ] && [ $GATE -eq 1 ]; then
    outputFile="gatetest"
elif [ "$EXTPROD" != "" ] && [ $EXTPROD -eq 1 ]; then
    outputFile="externalproducttest"
elif [ "$CMUX" != "" ] && [ $CMUX -eq 1 ]; then
    outputFile="cmuxtest"
elif [ "$BLINDROTATION" != "" ] && [ $BLINDROTATION -eq 1 ]; then
    outputFile="blindrotationtest"
elif [ "$gentestvect" != "" ] && [ $gentestvect -eq 1 ]; then
    outputFile="gentestvect"
elif [ "$BOOTSTRAPPING" != "" ] && [ $BOOTSTRAPPING -eq 1 ]; then
    outputFile="bootstrappingtest"
elif [ "$ALL" != "" ] && [ $ALL -eq 1 ]; then
    for i in $(cat run.sh |grep  "\--" |awk '{print $1}' |head -n -5 |awk -F \) '{print $1}');do 
        [ ! -d tests/${i/--/}test/test.txt ] & mkdir -p "tests/${i/--/}test";
        $0 $i >"tests/${i/--/}test/test.txt"; 
    done
    exit 0;
fi

g++ -g -o "${outputFile}.o" src/test/${outputFile}.cpp src/lwe/* src/tlwe/* src/tgsw/* src/general/* src/fft/SPQLIOS/{*avx.s,fft_processor_spqlios.cpp,spqlios-fft-impl.cpp} 
./${outputFile}.o
