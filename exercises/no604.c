#include<stdio.h>
#include<math.h>

int main()
{
    double r;
    scanf("%lf", &r);

    const double pi = 3.14159;

    double ans = pi * pow(r, 2);

    printf("A=%.4f", ans);
}