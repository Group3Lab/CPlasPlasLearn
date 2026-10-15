#include <iostream>

using namespace std;

void swap(int& a, int& b)//引用传递
{
    int t = a;
    a = b;
    b = t;
}

int main() 
{
    int a = 3, b = 8;
    swap(a, b);
    cout << "day7" << endl;
    cout << "a = " << a << ", b = " << b << endl;
    return 0;
}
