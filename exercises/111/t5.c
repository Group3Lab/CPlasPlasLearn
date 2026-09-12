
#include <stdio.h>

#include <string.h>

int countUpper(char *str)
{
    int ans = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] <= 'Z' && str[i] >= 'A')
            ans++;
    }
    return ans;
}


int main(void)
{
    char c[100];
    scanf("%s", c);

    printf("%d", countUpper(c));

    return 0;
}