#include <iostream>

using namespace std;

int main() 
{
    int score;
    cin >> score;

    if (score >= 90 && score <= 100)
    {
        cout << "优秀" << endl;
    }
    else if (score >= 60)
    {
        cout << "及格" << endl;
    }
    else
    {
        cout << "不及格" << endl;
    }

    return 0;
}
