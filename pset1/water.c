#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("minutes: ");
    int myNum = get_int();
    printf("Bottles: %i\n", myNum * 12);
}
