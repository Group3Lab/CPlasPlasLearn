#include <iostream>

using namespace std;

int main() 
{
    int ans = 0;
    int n;
    cin >> n;

    for (int i = 1; i <= n; i++)
    {
        ans += i;
    }

    cout << ans << endl;
    return 0;
}
