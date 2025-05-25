#include <stdio.h>
int power(int x, int y) {
    int result = 1;
    for (int i = 0; i < y; i++) {
        result *= x;
    }
    return result;
}

int main()
{
    int x = 2;
    int y = 3;
    printf("%d^%d = %d\n", x, y, power(x, y));
    return 0;
}