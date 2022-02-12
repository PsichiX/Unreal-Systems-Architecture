#!/bin/bash

rm -rf ../Distribution/
mkdir ../Distribution/

cp -rf ../Plugins/Systems ../Distribution/Systems-Architecture
rm -rf ../Distribution/Systems-Architecture/Binaries
rm -rf ../Distribution/Systems-Architecture/Intermediate
rm -rf ../Distribution/Systems-Architecture/Documentation
/c/Program\ Files/7-Zip/7z.exe a -tzip ../Distribution/Systems-Architecture.zip ../Distribution/Systems-Architecture
rm -rf ../Distribution/Systems-Architecture

cp -rf ../Plugins/SystemsUnitTests ../Distribution/Systems-Unit-Tests
rm -rf ../Distribution/Systems-Unit-Tests/Binaries
rm -rf ../Distribution/Systems-Unit-Tests/Intermediate
rm -rf ../Distribution/Systems-Unit-Tests/Documentation
/c/Program\ Files/7-Zip/7z.exe a -tzip ../Distribution/Systems-Unit-Tests.zip ../Distribution/Systems-Unit-Tests
rm -rf ../Distribution/Systems-Unit-Tests

cp -rf ../Plugins/SystemsQueryDebugger ../Distribution/Systems-Query-Debugger
rm -rf ../Distribution/Systems-Query-Debugger/Binaries
rm -rf ../Distribution/Systems-Query-Debugger/Intermediate
rm -rf ../Distribution/Systems-Query-Debugger/Documentation
/c/Program\ Files/7-Zip/7z.exe a -tzip ../Distribution/Systems-Query-Debugger.zip ../Distribution/Systems-Query-Debugger
rm -rf ../Distribution/Systems-Query-Debugger

cp -rf ../Plugins/ReactiveUserInterfaceSystems ../Distribution/Reactive-UI-Systems
rm -rf ../Distribution/Reactive-UI-Systems/Binaries
rm -rf ../Distribution/Reactive-UI-Systems/Intermediate
rm -rf ../Distribution/Reactive-UI-Systems/Documentation
/c/Program\ Files/7-Zip/7z.exe a -tzip ../Distribution/Reactive-UI-Systems.zip ../Distribution/Reactive-UI-Systems
rm -rf ../Distribution/Reactive-UI-Systems
