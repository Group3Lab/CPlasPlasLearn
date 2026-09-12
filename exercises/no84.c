/**
 * 求 1 + 2 + ... + n
 * 限制：不能用 乘除法、for、while、if、else、switch、case 及 A?B:C
 *
 * 思路：用递归代替循环，用逻辑与 && 的短路特性代替 if。
 *   - n != 0 时：n 为真，继续执行右边 sum += getSum(n - 1)，实现累加
 *   - n == 0 时：n 为假，&& 短路，不再递归，直接返回 sum(=0)
 */

#include <stdio.h>

int getSum(int n)
{
    int sum = n;
    (void)(n && (sum += getSum(n - 1))); /* 用 && 短路代替 if */
    return sum;
}

int main(void)
{
    for (int i = 0; i <= 10; i++) /* 这里的 for 只用于测试，不属于题解 */
    {
        printf("getSum(%2d) = %d\n", i, getSum(i));
    }
    return 0;
}