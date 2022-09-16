#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

glslc "$SCRIPT_DIR/shader.vert" -o "$SCRIPT_DIR/vert.spv"
glslc "$SCRIPT_DIR/shader.frag" -o "$SCRIPT_DIR/frag.spv"