#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    vector<int> v{60,70,80};

    v.push_back(90);

    // 只读遍历
    for (int x : v)
    {
        cout << x << " ";
    }

    cout << endl;

    // 引用遍历（可修改）
    for (int &x : v)
    {
        //x *= 2; // 修改元素
        cout << x << " ";
    }

    cout << endl;

    // const 引用（推荐，避免拷贝大对象）
    for (const auto &x : v)
    {
        cout << x << " ";
    }

    cout << endl;

    // 传统 for 循环
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }

    cout << endl;

    // 迭代器
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        cout << *it << " ";
    }

    return 0;
}
