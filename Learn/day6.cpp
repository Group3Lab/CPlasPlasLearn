#include <iostream>

using namespace std;

int getmax(int a,int b)
{
    return a > b ? a : b;
}

int main() 
{
    cout << getmax(3,8) << endl<<getmax(5,5);
    return 0;
}
