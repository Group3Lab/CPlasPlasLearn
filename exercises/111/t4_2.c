#include <stdio.h>

#define N 5

void print(const int a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }

    printf("\n");
}

int main(void)
{
    int n;
    scanf("%d", &n);

    int ppp[N + 1];                 // ppp[0] 空出来当临时位
    int maxid = 0, minid = 0;

    for (int i = 0; i < N; i++)
    {
        scanf("%d", &ppp[i + 1]);

        if (ppp[i + 1] > ppp[maxid])
            maxid = i + 1;
        if (ppp[i + 1] < ppp[minid])
            minid = i + 1;
    }

    int max = ppp[maxid];
    int min = ppp[minid];

    // 把最大值和最小值都挪到数组头部，一次遍历完成
    int k = 0;

    for (int i = 1; i <= N; i++)
    {
        if (i == maxid)
            ppp[k++] = max;         // 最大值固定放最前
        else if (i == minid)
            ppp[N] = min;           // 最小值暂存到末尾
    }

    // 剩余元素原序前移，填满 [1, N-2]
    for (int i = 1; i <= N; i++)
    {
        if (i != maxid && i != minid)
            ppp[k++] = ppp[i];
    }

    ppp[N - 1] = ppp[N];            // 最小值归位到倒数第二

    print(ppp, N);

    return 0;
}

//2 8 5 4 3
//8 5 4 3 2
