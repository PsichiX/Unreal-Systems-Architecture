#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
SERVE=0

while getopts "s" flag
do
    case "${flag}" in
        s) SERVE=1;;
    esac
done

unreal-doc -i "$SCRIPT_DIR/../Plugins/Systems/Documentation/UnrealDoc.toml"

if [ $SERVE = 1 ]
then
    mdbook serve -o "$SCRIPT_DIR/../Plugins/Systems/Documentation/Distribution"
fi