#include <stdio.h>

int find_decade(int a) {
    for (int i = 10; i < 1000000000; i = i * 10) {
        if (a / i == 0) {
            return i/10;
        }
    }
    return 0;
}
