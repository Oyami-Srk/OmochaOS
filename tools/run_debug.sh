#!/usr/bin/env bash

if [[ -z "${DEBUG_WAY}" ]]; then
  DEBUG_WAY="qemu"
fi

if [[ -z "${ROOT}" ]]; then
  ROOT="."
fi

qemu="qemu-system-i386"
bochs="bochs"


function run() {
    qemu-system-i386 -M q35 -m 512 -no-reboot -smp 1 -serial stdio -drive id=disk,file=build/HD.img,format=raw,if=none -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0
}

function debug() {
    qemu-system-i386 -m 512 -no-reboot -smp 1 -serial stdio -d cpu_reset,int,guest_errors -S -s -drive id=disk,file=build/HD.img,format=raw,if=none -drive file=tools/Test.img,format=raw,if=none -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0
}

"$@"
