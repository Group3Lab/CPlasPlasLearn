#include <iostream>
#include <vector>

using namespace std;

double average(const vector<int>& scores)
{
    if (scores.empty()) 
    {
        return 0;  // 空容器返回 0
    }

    double sum = 0;
    for (const auto& x : scores) {
        sum += x;
    }

    return sum / scores.size();  // 使用浮点数除法
}

int main()
{
    cout << "day19 - 平均值计算" << endl;

    // 测试1：对 {80, 90, 100} 返回 90
    vector<int> test1 = {80, 90, 100};
    cout << "对 {80, 90, 100} 返回 " << average(test1) << endl;

    // 测试2：对 {80, 81} 返回 80.5
    vector<int> test2 = {80, 81};
    cout << "对 {80, 81} 返回 " << average(test2) << endl;

    // 测试3：对空容器返回 0
    vector<int> test3 = {};
    cout << "对空容器返回 " << average(test3) << endl;

    return 0;
}
