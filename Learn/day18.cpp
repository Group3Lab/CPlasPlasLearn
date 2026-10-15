#include <iostream>
#include <memory>

using namespace std;

class Score
{
public:
    int value;
    Score(int v) : value(v) { cout << "Score 对象创建: " << v << endl; }
    ~Score() { cout << "Score 对象销毁: " << value << endl; }
};

int main()
{
    cout << "=== unique_ptr 演示 ===" << endl;
    cout << "unique_ptr: 独占所有权，只能有一个指针管理资源" << endl;
    {
        unique_ptr<Score> s1 = make_unique<Score>(85);
        cout << "s1 分数: " << s1->value << endl;

        // unique_ptr<Score> s2 = s1;  // 错误！不能拷贝
        unique_ptr<Score> s2 = move(s1);  // 正确，转移所有权
        cout << "转移所有权后，s2 分数: " << s2->value << endl;
        // cout << s1->value;  // 错误！s1 已经为空
    }  // s2 离开作用域，自动释放
    cout << "\n";


    cout << "=== shared_ptr 演示 ===" << endl;
    cout << "shared_ptr: 共享所有权，多个指针可以管理同一个资源" << endl;
    {
        // 创建一个分数，初值为 80
        shared_ptr<int> p = make_shared<int>(80);
        cout << "p 指向的分数: " << *p << endl;
        cout << "p 的引用计数: " << p.use_count() << endl;

        // q 也指向同一个分数
        shared_ptr<int> q = p;
        cout << "\nq = p，现在都指向同一个整数" << endl;
        cout << "p 的引用计数: " << p.use_count() << endl;
        cout << "q 的引用计数: " << q.use_count() << endl;

        // 修改分数
        *p = 90;
        cout << "\n通过 p 将分数改为 90" << endl;
        cout << "p 指向的分数: " << *p << endl;
        cout << "q 指向的分数: " << *q << endl;  // q 也看到了修改
        cout << "p 和 q 指向同一个整数！" << endl;

        cout << "\np 离开作用域" << endl;
    }  // q 也离开作用域，引用计数为 0，自动释放
    cout << "自动释放完成，无需手动 delete！" << endl;
    cout << "\n";


    cout << "=== weak_ptr 演示 ===" << endl;
    cout << "weak_ptr: 不增加引用计数，用于观察 shared_ptr" << endl;
    {
        shared_ptr<int> sp = make_shared<int>(100);
        cout << "shared_ptr 引用计数: " << sp.use_count() << endl;

        weak_ptr<int> wp = sp;
        cout << "weak_ptr 不增加引用计数: " << sp.use_count() << endl;

        // weak_ptr 不能直接访问，需要转换为 shared_ptr
        if (auto temp = wp.lock()) {  // lock() 返回 shared_ptr
            cout << "weak_ptr 可以转换为 shared_ptr，值为: " << *temp << endl;
        }

        // sp 释放后，wp 会变成无效
        sp.reset();
        cout << "sp 释放后，wp.lock() 返回: " << (wp.lock() ? "有效" : "无效") << endl;
    }
    cout << "\n";


    cout << "=== 完整示例：学生成绩管理 ===" << endl;
    {
        // 使用 shared_ptr 管理多个引用同一个成绩
        shared_ptr<int> studentScore = make_shared<int>(75);

        // 两个指针都指向这个成绩
        shared_ptr<int> p1 = studentScore;
        shared_ptr<int> p2 = studentScore;

        cout << "初始成绩: " << *p1 << endl;
        cout << "当前引用计数: " << studentScore.use_count() << endl;

        // 修改成绩
        *p2 = 88;
        cout << "\n修改后成绩: " << *p1 << endl;
        cout << "p1、p2、studentScore 都看到了修改" << endl;
    }
    cout << "作用域结束，自动释放成绩资源" << endl;

    return 0;
}
