#include<stdlib.h>
#include<string.h>

char *leftRotateString(char *str, int n)
{
    int len = (int)strlen(str);
    char *ans = malloc(len+n+1);

    strcpy(ans, str);

    char *defans = "";
    if(ans == NULL)
        return defans;

    for (int i = 0; i < n; i++)
    {
        *(ans + len + i) = *(ans + i);
    }

    *(ans + len + n) = '\0';

    ans = ans + n;

    return ans;
}

char *leftRotateString1(char *str, int n)
{
    int len = (int)strlen(str);
    if (len == 0)
        return NULL; /* 或返回空串，看题目约定 */

    n %= len; /* ① 归一化：n 超过 len 时取模 */

    char *base = malloc(len + 1); /* ② 只申请 len+1，长度不变 */
    if (base == NULL)
        return NULL;

    /* 先拷后半段 [n, len) 到开头 */
    memcpy(base, str + n, len - n);
    /* 再把前半段 [0, n) 接到后面 */
    memcpy(base + len - n, str, n);
    base[len] = '\0';

    return base; /* ③ 返回原始指针，调用者可 free */
}