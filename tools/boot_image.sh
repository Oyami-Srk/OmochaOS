#!/usr/bin/env bash
qemu="qemu-system-i386"
bochs="bochs"


function qemu_run() {
    ${qemu} -M q35 -m 512 -no-reboot -smp 1 -serial stdio -drive id=disk,file="$1",format=raw,if=none -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0 -d cpu_reset,int,guest_errors
}

function qemu_debug() {
    ${qemu} -m 512 -no-reboot -smp 1 -serial stdio -d cpu_reset,int,guest_errors -S -s -drive id=disk,file="$1",format=raw,if=none -drive file=tools/Test.img,format=raw,if=none -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0
}

function qemu() {
  if [[ "${1,,}" == "debug" ]]; then
    qemu_debug "$2"
  else
    qemu_run "$2"
  fi
}

"$@"
