#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BINARIES=0

while getopts "b" flag
do
    case "${flag}" in
        b) BINARIES=1;;
    esac
done

rm -rf "$SCRIPT_DIR/../Distribution/"
mkdir "$SCRIPT_DIR/../Distribution/"

cp -rf "$SCRIPT_DIR/../Plugins/Systems" "$SCRIPT_DIR/../Distribution/Systems-Architecture"
if [ $BINARIES = 0 ]
then
    rm -rf "$SCRIPT_DIR/../Distribution/Systems-Architecture/Binaries"
fi
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Architecture/Intermediate"
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Architecture/Documentation"
"/c/Program Files/7-Zip/7z.exe" a -tzip "$SCRIPT_DIR/../Distribution/Systems-Architecture.zip" "$SCRIPT_DIR/../Distribution/Systems-Architecture"
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Architecture"

cp -rf "$SCRIPT_DIR/../Plugins/SystemsUnitTests" "$SCRIPT_DIR/../Distribution/Systems-Unit-Tests"
if [ $BINARIES = 0 ]
then
    rm -rf "$SCRIPT_DIR/../Distribution/Systems-Unit-Tests/Binaries"
fi
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Unit-Tests/Intermediate"
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Unit-Tests/Documentation"
"/c/Program Files/7-Zip/7z.exe" a -tzip "$SCRIPT_DIR/../Distribution/Systems-Unit-Tests.zip" "$SCRIPT_DIR/../Distribution/Systems-Unit-Tests"
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Unit-Tests"

cp -rf "$SCRIPT_DIR/../Plugins/SystemsSpatialQuery" "$SCRIPT_DIR/../Distribution/Systems-Spatial-Query"
if [ $BINARIES = 0 ]
then
    rm -rf "$SCRIPT_DIR/../Distribution/Systems-Spatial-Query/Binaries"
fi
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Spatial-Query/Intermediate"
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Spatial-Query/Documentation"
"/c/Program Files/7-Zip/7z.exe" a -tzip "$SCRIPT_DIR/../Distribution/Systems-Spatial-Query.zip" "$SCRIPT_DIR/../Distribution/Systems-Spatial-Query"
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Spatial-Query"

cp -rf "$SCRIPT_DIR/../Plugins/SystemsQueryDebugger" "$SCRIPT_DIR/../Distribution/Systems-Query-Debugger"
if [ $BINARIES = 0 ]
then
    rm -rf "$SCRIPT_DIR/../Distribution/Systems-Query-Debugger/Binaries"
fi
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Query-Debugger/Intermediate"
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Query-Debugger/Documentation"
"/c/Program Files/7-Zip/7z.exe" a -tzip "$SCRIPT_DIR/../Distribution/Systems-Query-Debugger.zip" "$SCRIPT_DIR/../Distribution/Systems-Query-Debugger"
rm -rf "$SCRIPT_DIR/../Distribution/Systems-Query-Debugger"

cp -rf "$SCRIPT_DIR/../Plugins/ReactiveUserInterfaceSystems" "$SCRIPT_DIR/../Distribution/Reactive-UI-Systems"
if [ $BINARIES = 0 ]
then
    rm -rf "$SCRIPT_DIR/../Distribution/Reactive-UI-Systems/Binaries"
fi
rm -rf "$SCRIPT_DIR/../Distribution/Reactive-UI-Systems/Intermediate"
rm -rf "$SCRIPT_DIR/../Distribution/Reactive-UI-Systems/Documentation"
"/c/Program Files/7-Zip/7z.exe" a -tzip "$SCRIPT_DIR/../Distribution/Reactive-UI-Systems.zip" "$SCRIPT_DIR/../Distribution/Reactive-UI-Systems"
rm -rf "$SCRIPT_DIR/../Distribution/Reactive-UI-Systems"
