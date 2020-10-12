#!/usr/bin/env bash

# attach disk image file
uname=$(uname)

# function attach_macos mount disk image on macos
# require argument: 1.disk image file path
# return: 1. mount path
# side effect:
function attach_macos() {
    local boot_image_mount_dir = `hdiutil mount $1 | grep '/Volumes/.*$$' -o`
    echo $boot_image_mount_dir
}

# function attach call attach_x function for
# each platform.
# require argument: 1.disk image file path
# return: 1. mount path
function attach() {
    if [ ! -f "$1" ]; then
        1>&2 echo "disk image not exists."
        return 1
    fi
    case $uname in
        "Darwin" )
            echo "Darwin"
            ;;
        "Linux" )
            echo "Linux"
            ;;
    esac
}

"$@"
