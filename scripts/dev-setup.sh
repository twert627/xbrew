#!/bin/bash

check_internet() {
    # Credit to <https://stackoverflow.com/questions/17291233/how-can-i-check-internet-access-using-a-bash-script-on-linux>
    echo "Checking for internet..."
    wget -q --tries=10 --timeout=20 --spider http://github.com
    if [[ $? -eq 0 ]]; then
        echo "Online"
        echo ""
    else
        echo "Offline! Please check internet connection and try again..."
        return 1
    fi
}

install_packages() {
    echo "Installing packages..."

    # TODO: Add RedHat and Fedora support
    case $DISTRIB in
    ubuntu)
        echo "Installing required packages to compile xbrew on Ubuntu..."
        sudo apt-get update && sudo apt-get install build-essential curl # Ubuntu includes build-essential for compiling C code, curl for networking
        echo "Checking installed packages..."
        gcc -v
        make -v
        ;;
    debian) 
        echo "Installing required packages to compile xbrew on Debian..."
        sudo apt-get update && sudo apt-get install build-essential curl # Debian and Ubuntu use same package manager :)
        echo "Checking installed packages..."
        gcc -v
        make -v
        ;;
    arch)
        echo "Installing required packages to compile xbrew on Arch..."
        sudo pacman -Syy && sudo pacman -S gcc make 
        echo "Checking installed packages..."
        gcc -v
        make -v
        ;;
    darwin)
        echo "Installing required packages to compile xbrew on MacOSX..."
        # Never used mac, someone elses problem lol
        gcc -v
        make -v
        echo "Not implemented yet!!"
        ;;
    esac
}

check_os() {
    # Credit to <https://gist.github.com/gmolveau/d0e3efc219c5bcc6ecc13a1405ac6c73>
    echo "Checking Operating System..."
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        DISTRIB=$(awk -F'=' '/^ID=/ {print tolower($2)}' /etc/*-release 2>/dev/null)
        if [[ ${DISTRIB} = "ubuntu"* ]]; then
            if uname -a | grep -q '^linux.*microsoft'; then
                echo "Ubuntu via WSL"
            else
                echo "Ubuntu"
            fi
        elif [[ ${DISTRIB} = "debian"* ]]; then
            echo "Debian"
        elif [[ ${DISTRIB} = "arch"* ]]; then
            echo "Arch"
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "macOS OSX"
    else
        echo "Error, unsupported operating system"
    fi
    echo ""
}

main() {
    echo "=== Linux (bash) xbrew dev setup script (by twert) ==="
    echo ""
    check_internet
    check_os
    install_packages
    echo "Compile away! bwahaha"
}
main
