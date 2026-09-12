#include<stdio.h>

int main(void)
{
    int num;
    scanf("%d", &num);

    num = num > 0 ? num : -num;

    int ans1 = 0, ans2 = 0;

    for (; num > 0; num /= 10)
    {
        //printf("%d ", num % 10);

        int x = num % 10;

        if(x<=5)
            continue;

        ans1 += x;
        ans2 += 1;
    }

    printf("%d %d", ans1, ans2);

    return 0;
}