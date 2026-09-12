#include<stdio.h>

void swap(int *p1,int *p2)
{
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int main(void)
{
    int a, b;
    scanf("%d %d", &a, &b);

    swap(&a, &b);

    printf("%d,%d", a, b);

    return 0;
}