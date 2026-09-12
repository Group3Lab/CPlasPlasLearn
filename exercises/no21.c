#include<stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    int ans[50];

    ans[1] = 1;
    ans[2] = 1;

    for (int i = 3; i <= n; i++)
    {
        ans[i] = ans[i - 1] + ans[i - 2];
    }

    printf("%d", ans[n]);

    return 0;
}