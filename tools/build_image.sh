#!/usr/bin/env bash

UNAME=$(uname)
TOOLS="$(pwd)/tools"
BUILD="$(pwd)/build"

TEMPLATE_FILES="$TOOLS/HD_grub2.img"
IMAGE_FILE="$TOOLS/HD.img"
GRUB2_CONF="$TOOLS/grub.cfg"

echo "Script running under $UNAME"

# usage: ./build_image prepare HD.img
function prepare() {
    if [[ ! -f $1 ]]; then
      cp "$TEMPLATE_FILES" "$1"
    else
      echo "$1 already exists!"
    fi
}

function attach() {
  if [[ -f "$1.lock" ]]; then
    echo "You have already mounted image $1"
    exit 1
  fi
  mount_dir=""
  if [[ "x$UNAME" == "xDarwin" ]]; then
    echo "Use hdiutil to mount image " "$1"
    mount_dir="$(hdiutil mount "$1" | grep '/Volumes/.*$' -o)"
  elif [[ "x$UNAME" == "xLinux" ]]; then
    echo "Use udisk2 to mount image, make sure you could use it."
    local loop_device
    loop_device="$(udisksctl loop-setup -f "$1" | grep -o -e '/dev/loop[0-9]*')"
    echo "$loop_device" > "$1.lock.loop"
    mount_dir="$(udisksctl mount -b "${loop_device}"p1 | sed -n -e "s/^.* at \(\/.*\)$/\1/p")"
    echo "Image file mount dir: $mount_dir"
  fi
  echo "$mount_dir" > "$1.lock"
}

function detach() {
  if [[ ! -f "$1.lock" ]]; then
    echo "You have already unmounted image $1"
    exit 2
  fi
  mount_dir=$(cat "$1.lock")
  if [[ "x$UNAME" == "xDarwin" ]]; then
    echo "Use hdiutil to unmount imag $1"
    hdiutil eject "$(cat "$1.lock")"
    rm "$1.lock"
  elif [[ "x$UNAME" == "xLinux" ]]; then
    umount "$mount_dir"
    udisksctl loop-delete -b "$(cat "$1.lock.loop")"
    rm "$1.lock.loop"
    rm "$1.lock"
  fi
}

# usage: ./build_image copy_to HD.img /a/b/c/ a.bin
function copy_to() {
  if [[ ! -f "$1.lock" ]]; then
    echo "Disk image $1 is not mounted."
    exit 3
  fi
  mount_dir=$(cat "$1.lock")
  cp "$3" "$mount_dir/$2"
  echo "Copied $3 to ${mount_dir}/$2"
}

# usage: ./build_image sync_to HD.img /a/b/c/ d/
function sync_to() {
  if [[ ! -f "$1.lock" ]]; then
    echo "Disk image $1 is not mounted."
    exit 4
  fi
  mount_dir=$(cat "$1.lock")
  rsync -avh --progress "$3" "$mount_dir/$2"
}

# usage ./build_image all HD.img kernel.elf grub.cfg root_dir/
function all() {
  prepare "$1"
  attach "$1"
  copy_to "$1" /kernel.out "$2"
  copy_to "$1" /boot/grub/ "$3"
  sync_to "$1" / "$4"
  detach "$1"
}

if [[ ! -f "$2" ]]; then
#  echo "You must specified an image to use this script."
#  exit 1
  prepare $2
fi

"$@"
