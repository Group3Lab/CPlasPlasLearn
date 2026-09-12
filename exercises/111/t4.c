#include <stdio.h>
#include <stdlib.h>

int ppp[100];

void print()
{
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", ppp[i]);
    }

    printf("\n");
}

int main(void)
{
    int n;
    scanf("%d", &n);

    //int *ppp = (int *)malloc((5+2) * sizeof(int));
    //if(ppp == NULL)
    //    return 0;

    int maxid = 1, minid = 1;

    for (int i = 1; i <= 5; i++)
    {
        int x;
        scanf("%d", &x);

        ppp[i] = x;

        if(ppp[i] > ppp[maxid])
            maxid = i;
        if (ppp[i] < ppp[minid])
            minid = i;
    }

    //print();

    int max = ppp[maxid];
    int min = ppp[minid];

    ppp[0] = max;

    for (int i = 1; i <= 5; i++)
    {
        if(i == maxid || i == minid)
        {
            for (int j = i; j <= 5-1; j++)
            {
                ppp[j] = ppp[j + 1];
            }

            if(i == maxid)
            {
                maxid = minid;
            }
            if (i == minid)
            {
                minid = maxid;
            }

            break;
        }
    }

    minid--;
    maxid--;

    for (int i = 1; i <= 5; i++)
    {
        if (i == maxid || i == minid)
        {
            for (int j = i; j <= 5 - 1; j++)
            {
                ppp[j] = ppp[j + 1];
            }

            break;
        }
    }

    ppp[5-1] = min;

    print();

    //free(ppp);

    return 0;
}

//2 8 5 4 3
//8 5 4 3 2