param (
    [switch]$binaries = $false,
    [switch]$archive = $false
)

$source = "$PSScriptRoot/../Plugins"
$target = "$PSScriptRoot/../Distribution"

function New-Package {
    param (
        [string]$name,
        [string]$dir
    )

    $source = "$source/$name"
    $target = "$target/$dir"

    Copy-Item $source $target -Recurse -Force
    if ($binaries) {
        Get-ChildItem -Path "$target/Binaries" -Recurse -Include "*.pdb" | ForEach-Object {
            Remove-Item "$_" -Force
        }
    } else {
        Remove-Item "$target/Binaries" -Recurse -Force
    }
    Remove-Item "$target/Intermediate" -Recurse -Force
    Remove-Item "$target/Documentation" -Recurse -Force -ErrorAction SilentlyContinue
    if ($archive) {
        7z a -tzip "$target.zip" $target
        Remove-Item $target -Recurse -Force
    }
}

Remove-Item $target -Recurse -Force
New-Item $target -ItemType Directory | Out-Null

New-Package -name "Systems" -dir "Systems-Architecture"
New-Package -name "SystemsUnitTests" -dir "Systems-Unit-Tests"
New-Package -name "SystemsSpatialQuery" -dir "Systems-Spatial-Query"
New-Package -name "SystemsQueryDebugger" -dir "Systems-Query-Debugger"
New-Package -name "ReactiveUserInterfaceSystems" -dir "Reactive-UI-Systems"
