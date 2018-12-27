#include "elf.h"
#include "memory_map.h"


int main(){
  // here should goes FS module, loading kernel and boot
  // but we have no FS module here, so we just skip stage2
  // and directly put our kernel into sector 1
  // until ........ (the end of world) we have it.
  while(1);
}
