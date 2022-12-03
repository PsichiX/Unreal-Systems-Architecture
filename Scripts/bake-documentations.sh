#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

unreal-doc -i "$SCRIPT_DIR/../Plugins/Systems/Documentation/UnrealDoc.toml"
