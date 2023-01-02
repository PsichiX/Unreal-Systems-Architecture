#!/usr/bin/env powershell

param (
    [switch]$serve = $false
)

unreal-doc -i "$PSScriptRoot/../Plugins/Systems/Documentation/UnrealDoc.toml"

if ($serve) {
    mdbook serve -o "$PSScriptRoot/../Plugins/Systems/Documentation/Distribution"
}
