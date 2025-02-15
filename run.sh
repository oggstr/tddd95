#!/bin/bash

# -- Init
# Directory script is called from
CALLING_DIR=$(pwd)

# Directory of script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

on_enter() {
    cd $SCRIPT_DIR
}

on_exit() {
    cd $CALLING_DIR
}

# Allows calling from anywhere
on_enter
trap on_exit EXIT

# -- Functions

usage() {
    echo "run.sh [command] [args..]"
    echo ""
    echo ""
    echo "Commands:"
    echo ""
    echo "exercise|ex <sub-command> <file_name>"
    echo "lab <sub-command> <file_name>"
    echo ""
    echo ""
    echo "Sub command:"
    echo ""
    echo "comp - compile"
    echo "run - run"
    echo "comprun - compile and run"
}

# Compile and run
# Args:
# - path
comp_and_run() {
    comp $1 $2
    run $1 $2
}

comp() {
    if [ "$#" -ne 2 ]; then
        echo "Expected 2 args, got: $#"
        exit 1
    fi

    local path="$1"
    local file="$2"
    g++-14 -O2 -o "$path/sol" -std=gnu++23 -static -lrt -Wl,--whole-archive -lpthread -Wl,--no-whole-archive "$path/$file.cpp"

    #g++-14 -O2 -o "$path/sol" -std=gnu++23 "$path/$file.cpp"
    #clang++ -O2 -o "$path/sol" -std=c++23 -pthread "$path/$file.cpp"
}

run() {
    if [ "$#" -ne 2 ]; then
        echo "Expected 2 args, got: $#"
        exit 1
    fi

    local path="$1"
    local file="$2"
    $path/sol < "$SCRIPT_DIR/input/$file.txt"
}


# -- Commands

if [ $# -eq 0 ]; then
    echo "Expected one command - run.sh help for help"
    exit 1
fi

case $1 in
    help)
        usage
    ;;
    exercise|ex)
        if [ $# -ne 3 ]; then
            echo "Expected 3 arguments, got: $#"
            exit 1
        fi

        code_path="$SCRIPT_DIR/exercises"
        file_name=$3
    ;;
    lab)
        if [ $# -ne 3 ]; then
            echo "Expected 3 arguments, got: $#"
            exit 1
        fi
    ;;
    *)
        echo "Unknown command"
        exit 1
    ;;
esac

case $2 in

    comp)
        comp $code_path $file_name
    ;;
    run)
        run $code_path $file_name
    ;;
    diff)
        run $code_path $file_name | diff - "$SCRIPT_DIR/output/$file_name.txt"
    ;;
    comprun)
        comp_and_run $code_path $file_name
    ;;
    *)
        echo "Unknown sub command"
        exit 1
    ;;

esac