#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main()
{
    // ===== 成绩升序排列 =====
    cout << "=== 成绩升序排列 ===" << endl;
    vector<int> scores = {80, 60, 95, 70, 85};

    cout << "排序前: ";
    for (int s : scores) {
        cout << s << " ";
    }
    cout << endl;

    sort(scores.begin(), scores.end());  // 升序排列

    cout << "排序后: ";
    for (int s : scores) {
        cout << s << " ";
    }
    cout << "\n\n";


    // ===== 认识 pair =====
    cout << "=== pair 的使用 ===" << endl;
    pair<string, int> student1("Alice", 90);
    pair<string, int> student2 = make_pair("Bob", 85);

    cout << "学生1: " << student1.first << ", 成绩: " << student1.second << endl;
    cout << "学生2: " << student2.first << ", 成绩: " << student2.second << endl;
    cout << "\n";


    // ===== 认识 map =====
    cout << "=== map 的使用 ===" << endl;
    map<string, int> studentScores;

    // 插入数据
    studentScores["Alice"] = 90;
    studentScores["Bob"] = 85;
    studentScores["Charlie"] = 95;
    studentScores.insert(make_pair("David", 88));

    // 遍历 map
    cout << "所有学生成绩:" << endl;
    for (const auto& [name, score] : studentScores) {  // C++17 结构化绑定
        cout << name << ": " << score << endl;
    }
    cout << "\n";


    // ===== find 的使用 =====
    cout << "=== find 查找元素 ===" << endl;

    // 在 vector 中查找
    auto it1 = find(scores.begin(), scores.end(), 85);
    if (it1 != scores.end()) {
        cout << "在 vector 中找到 85" << endl;
    }

    // 在 map 中查找
    auto it2 = studentScores.find("Bob");
    if (it2 != studentScores.end()) {
        cout << "找到 Bob, 成绩: " << it2->second << endl;
    }

    auto it3 = studentScores.find("Eve");
    if (it3 == studentScores.end()) {
        cout << "没有找到 Eve" << endl;
    }
    cout << "\n";


    // ===== count 的使用 =====
    cout << "=== count 统计元素 ===" << endl;
    vector<int> numbers = {1, 2, 3, 2, 4, 2, 5};

    int count2 = count(numbers.begin(), numbers.end(), 2);
    cout << "数字 2 出现了 " << count2 << " 次" << endl;

    // map 的 count（只能是 0 或 1，因为键唯一）
    cout << "map 中是否有 Alice: " << studentScores.count("Alice") << endl;
    cout << "map 中是否有 Eve: " << studentScores.count("Eve") << endl;
    cout << "\n";


    // ===== sort 的更多用法 =====
    cout << "=== sort 降序排列 ===" << endl;
    vector<int> scores2 = {80, 60, 95, 70, 85};

    sort(scores2.begin(), scores2.end(), greater<int>());  // 降序

    cout << "降序排列: ";
    for (int s : scores2) {
        cout << s << " ";
    }
    cout << "\n\n";


    // ===== 自定义排序 =====
    cout << "=== 自定义排序规则 ===" << endl;
    vector<pair<string, int>> students = {
        {"Alice", 85},
        {"Bob", 90},
        {"Charlie", 85},
        {"David", 95}
    };

    // 按成绩降序，成绩相同按名字升序
    sort(students.begin(), students.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) {
            return a.second > b.second;  // 成绩降序
        }
        return a.first < b.first;  // 名字升序
    });

    cout << "排序后:" << endl;
    for (const auto& [name, score] : students) {
        cout << name << ": " << score << endl;
    }

    return 0;
}
