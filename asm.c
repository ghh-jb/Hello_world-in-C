#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int summ(int a, int b) {
    int rax = 0;
    int rdx = 0;
    asm(
        "add %2, %1\n"
        : "=a" (rax)
        : "a" (a), "d" (b)
    );
    return rax;
}

int main() {
    int a,b;
    scanf("%i", &a);
    scanf("%i", &b);
    int ret = summ(a, b);
    printf("[summ] %i\n", ret);
    return 0;

}