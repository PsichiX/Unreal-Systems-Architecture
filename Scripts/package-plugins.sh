#!/bin/bash

rm -rf ../Distribution/
/c/Program\ Files/7-Zip/7z.exe a -tzip ../Distribution/Systems-Architecture.zip ../Plugins/Systems
/c/Program\ Files/7-Zip/7z.exe a -tzip ../Distribution/Systems-Query-Debugger.zip ../Plugins/SystemsQueryDebugger
/c/Program\ Files/7-Zip/7z.exe a -tzip ../Distribution/Reactive-UI-Systems.zip ../Plugins/ReactiveUserInterfaceSystems