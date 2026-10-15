
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Student
{
    string name;
    int score;
};

int countPassed(const vector<Student>& students)
{
    int count = 0;
    for (size_t i = 0; i < students.size(); ++i)  // 从 0 开始，否则会漏掉第一个学生
    {
        if (students[i].score >= 60)
        {
            ++count;
        }
    }
    return count;
}

int main()
{
    cout << "day20 - 统计及格人数" << endl;

    vector<Student> students{
        {"A", 80}, {"B", 55}, {"C", 90}
    };

    // 预期输出 2（A 和 C 及格）
    cout << countPassed(students) << endl;

    return 0;
}
