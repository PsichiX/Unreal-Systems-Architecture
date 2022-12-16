#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BINARIES=0

while getopts "b" flag
do
    case "${flag}" in
        b) BINARIES=1;;
    esac
done

function package {
    local NAME="$1"
    local DIR="$2"

    cp -rf "$SCRIPT_DIR/../Plugins/$NAME" "$SCRIPT_DIR/../Distribution/$DIR"
    if [ $BINARIES = 1 ]
    then
        find "$SCRIPT_DIR/../Distribution/$DIR/Binaries" -name "*.pdb" -type f -delete
    else
        rm -rf "$SCRIPT_DIR/../Distribution/$DIR/Binaries"
    fi
    rm -rf "$SCRIPT_DIR/../Distribution/$DIR/Intermediate"
    rm -rf "$SCRIPT_DIR/../Distribution/$DIR/Documentation"
    "/c/Program Files/7-Zip/7z.exe" a -tzip "$SCRIPT_DIR/../Distribution/$DIR.zip" "$SCRIPT_DIR/../Distribution/$DIR"
    rm -rf "$SCRIPT_DIR/../Distribution/$DIR"
}

rm -rf "$SCRIPT_DIR/../Distribution/"
mkdir "$SCRIPT_DIR/../Distribution/"

package "Systems" "Systems-Architecture"
package "SystemsUnitTests" "Systems-Unit-Tests"
package "SystemsSpatialQuery" "Systems-Spatial-Query"
package "SystemsQueryDebugger" "Systems-Query-Debugger"
package "ReactiveUserInterfaceSystems" "Reactive-UI-Systems"
