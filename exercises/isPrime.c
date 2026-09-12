#include<stdio.h>
#include<stdbool.h>

bool isPrime(int x);

void output(int n)
{
    bool flag = 1;

    for (int i = 11; i < n; i += 10)
    {
        if (isPrime(i))
        {
            printf("%d ", i);
            flag = 0;
        }
    }

    if (flag)
        printf("-1");

    printf("\n");
}

int main(void)
{
    for (;;)
    {
        int x = 0;
        scanf("%d", &x);

        if(x == 0)
            break;

        output(x);
    }
    

    return 0;
}

bool isPrime(int x)
{
    for (int i = 2; i <= x/i; i++)
    {
        if(x%i == 0)
            return 0;
    }

    return 1;
}
