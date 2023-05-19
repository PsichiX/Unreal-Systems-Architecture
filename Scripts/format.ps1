#!/usr/bin/env powershell

function Format-Sources
{
    param (
        [string]$path
    )

    Get-ChildItem "$path" -Recurse -Include *.h, *.cpp | ForEach-Object {
        Write-Output "- $_"
        clang-format -i "$_"
    }
    Write-Output ""
}

Format-Sources -path "$PSScriptRoot/../Source"
Format-Sources -path "$PSScriptRoot/../Plugins/Systems/Source"
Format-Sources -path "$PSScriptRoot/../Plugins/Systems/Documentation"
Format-Sources -path "$PSScriptRoot/../Plugins/SystemsQueryDebugger/Source"
Format-Sources -path "$PSScriptRoot/../Plugins/SystemsUnitTests/Source"
Format-Sources -path "$PSScriptRoot/../Plugins/SystemsSpatialQuery/Source"
Format-Sources -path "$PSScriptRoot/../Plugins/ReactiveSystems/Source"
