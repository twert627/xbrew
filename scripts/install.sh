#!/bin/bash

main() {
    echo "=== xbrew installer script (by twert) ==="
    cp ./build/xbrew /usr/bin
    if [[ -f /usr/bin/xbrew ]]; then
        echo ""
        echo "xbrew installed!"
    else 
        echo ""
        echo "xbrew not installed! (did you run as sudo?)"
    fi
}
main