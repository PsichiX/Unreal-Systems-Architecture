#!/usr/bin/env powershell

Get-ChildItem -Recurse -Include Source | ForEach-Object {
    Get-ChildItem "$_" -Recurse -Include *.h, *.cpp | ForEach-Object {
        Write-Output "- $_"
        clang-format -i "$_"
    }
    Write-Output ""
}