#include "modules/tty.h"
int vaddr;

int main(int argc, char **argv) {
    printf("Hi, this is your first app!\n");
    printf("Argc: %d, argv(%x): ", argc, argv);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
    vaddr = 124195;
    printf("v: %d\n", vaddr);
    return 233;
}
