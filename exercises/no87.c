#include <string.h>
#include <limits.h>

//边界情况太多了。。。。


/**
 * strToInt: 字符串转整数（不借助 atoi 等库函数）
 *  1. 跳过行首空格
 *  2. 可选 '+' / '-'
 *  3. 取最长的连续数字串
 *  4. 无数字则返回 0
 *  5. 越界则返回 INT_MAX / INT_MIN
 */
int strToInt(char *str)
{
    int len = (int)strlen(str);
    int i = 0;

    /* ① 跳过行首空格（i < len 放前面，短路防越界） */
    while (i < len && str[i] == ' ')
    {
        i++;
    }

    /* ② 处理符号位 */
    int op = 1;
    if (i < len && (str[i] == '+' || str[i] == '-'))
    {
        if (str[i] == '-')
        {
            op = -1;
        }
        i++;
    }

    /* ③ 累加数字，全程按"负数"累加，避免 INT_MIN 无法直接表示的问题 */
    int ans = 0;    /* 存的是"结果的负值"：例如结果 123 时 ans = -123 */
    int found = 0;  /* 是否至少读到一个数字 */

    while (i < len && str[i] >= '0' && str[i] <= '9')
    {
        int digit = str[i] - '0';
        found = 1;

        /* ④ 溢出判断：在乘之前判断，而不是乘完再看符号 */
        /* 限制统一按 INT_MIN（= -2147483648）来比较 */
        int limit = INT_MIN / 10;          /* -214748364 */
        if (ans < limit || (ans == limit && digit > 8))
        {
            /* 已经确定会越界，直接返回边界值 */
            return (op > 0) ? INT_MAX : INT_MIN;
        }

        ans = ans * 10 - digit;
        i++;
    }

    /* ⑤ 一个数字都没有 → 返回 0 */
    if (!found)
    {
        return 0;
    }

    /* ⑥ 负数？直接就是结果；正数？取相反数并检查上界 */
    if (op < 0)
    {
        return ans;                     /* ans 已经是负数 */
    }

    if (ans == INT_MIN)
    {
        return INT_MAX;                 /* 正方向装不下 INT_MIN 的绝对值 */
    }

    return -ans;
}
