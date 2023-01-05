#!/usr/bin/env powershell

Get-ChildItem "$PSScriptRoot/.." -Recurse -Include Binaries, Intermediate | ForEach-Object {
    Remove-Item "$_" -Recurse -Force
    Write-Output "Deleted: $_"
}