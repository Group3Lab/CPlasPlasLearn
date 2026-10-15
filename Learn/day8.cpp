#include <iostream>

using namespace std;

int getMax(int a[], int size)
{
    int max = a[0];
    for (int i = 1; i < size; i++)
    {
        if (a[i] > max)
            max = a[i];
    }
    return max;
}

int main() 
{
    int a[20] = {80,60,95,70,85};

    cout << getMax(a, 5) << endl;

    return 0;
}
