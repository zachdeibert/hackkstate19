#!/bin/bash
set -e

cd "$(dirname "$0")/build"
(clear; echo -e "Filament\nDetector" | toilet --gay -w 100; echo "By Miranda Dodson and Zach Deibert") | sudo tee /dev/tty1 > /dev/null
./printer-controller $(cat ~/options.txt)
