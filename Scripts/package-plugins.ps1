#!/usr/bin/env powershell

$source = "$PSScriptRoot/../Plugins"
$target = "$PSScriptRoot/../Distribution"

function New-Package {
    param (
        [string]$name,
        [string]$dir,
        [switch]$blueprintonly
    )

    $source = "$source/$name"
    $target = "$target/$dir"

    Copy-Item $source $target -Recurse -Force
    if ($blueprintonly) {
        Get-ChildItem -Path "$target/Binaries" -Recurse -Include "*.pdb" | ForEach-Object {
            Remove-Item "$_" -Force
        }
    }
    else {
        Remove-Item "$target/Binaries" -Recurse -Force
    }
    Remove-Item "$target/Intermediate" -Recurse -Force
    Remove-Item "$target/Documentation" -Recurse -Force -ErrorAction SilentlyContinue
    if ($blueprintonly) {
        Remove-Item "$target/Source" -Recurse -Force
    }
    7z a -tzip "$target.zip" $target
    Remove-Item $target -Recurse -Force
}

function New-Package-Set {
    param (
        [string]$name,
        [string]$dir
    )

    New-Package -name $name -dir $dir
    New-Package -name $name -dir "$dir-BP" -b
}

Remove-Item $target -Recurse -Force
New-Item $target -ItemType Directory | Out-Null

New-Package-Set -name "Systems" -dir "Systems-Architecture"
New-Package -name "SystemsUnitTests" -dir "Systems-Unit-Tests"
New-Package-Set -name "SystemsSpatialQuery" -dir "Systems-Spatial-Query"
New-Package-Set -name "SystemsQueryDebugger" -dir "Systems-Query-Debugger"
New-Package-Set -name "ReactiveUserInterfaceSystems" -dir "Reactive-UI-Systems"
