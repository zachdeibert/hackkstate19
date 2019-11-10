#!/bin/bash
set -e

cd "$(dirname "$0")/build"
./printer-controller $(cat ~/options.txt)
