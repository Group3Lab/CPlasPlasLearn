#include <string.h>
#include <stdlib.h>

char *replaceSpaces(char *str)
{
    int len = (int)strlen(str);
    char *ans = malloc(len * 3 + 1); // 最坏情况：每个字符都是空格
    if (ans == NULL)
    {
        return NULL;
    }
    int i = 0;

    for (char *now = str; *now != '\0'; now++)
    {
        if (*now == ' ')
        {
            ans[i++] = '%';
            ans[i++] = '2';
            ans[i++] = '0';
        }
        else
        {
            ans[i++] = *now;
        }
    }
    ans[i] = '\0';

    return ans; /* 由调用者负责 free */
}