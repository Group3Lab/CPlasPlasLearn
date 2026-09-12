#include<stdio.h>

void getStats(int arr[], int n, int *pSum, int *pAvg, int *pCnt)
{
    *pSum = 0;
    *pCnt = 0;

    int c = 0;

    for (int i = 0; i < n; i++)
    {
        int x = arr[i];
        *pSum += x;
    }
    *pAvg = *pSum / n;

    for (int i = 0; i < n; i++)
    {
        int x = arr[i];
        if (x>*pAvg)
        {
            c++;

            //printf("%d,", x);
        }
    }

    *pCnt = c;
}

int main()
{
    int n;
    scanf("%d", &n);

    int aa[20];

    for (int i = 0; i < n; i++)
    {
        int x;
        scanf("%d", &x);

        aa[i] = x;
    }

    int a, b, c;

    getStats(aa, n, &a, &b, &c);

    printf("%d %d %d", a, b, c);
}