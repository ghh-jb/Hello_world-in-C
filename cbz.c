#include <stdio.h>
#include <stdbool.h>
bool cbz(int val) {
    int rax = 0;
    asm(
        "cmp $0, %%rdx\n"
        "jz zero\n"
        "mov $0, %%rax\n"
        "jmp end\n"
        "zero:\n"
        "mov $1, %%rax\n"
        "end:"
        : "=a" (rax)
        : "d" (val)
    );
    printf("[val:rax] %i:%i\n", val, rax);
    return rax;
}

int main() {
    cbz(1);
    cbz(0);
    return 0;
}