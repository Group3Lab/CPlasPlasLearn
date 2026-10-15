// ============================================================================
// day9：C++ 面向对象（OOP）速成 —— 全部对照 C# 讲解
// ----------------------------------------------------------------------------
// 建议按 main() 里的编号顺序读，每一节都先讲「C# 怎么写」再讲「C++ 怎么写」。
// ============================================================================
#include <iostream>   // cout / endl
#include <string>     // string：C++ 的 std::string，对应 C# 的 string
#include <vector>     // vector：C++ 的动态数组，对应 C# 的 List<T>
#include <memory>     // ★ 智能指针 unique_ptr / shared_ptr / weak_ptr 都在这里

using namespace std;   // C# 靠 using 指令简化命名空间，C++ 也可以用（小项目里常见）

// ============================================================================
// 一、类的定义：从 C# 到 C++
// ----------------------------------------------------------------------------
// C#： class Student { private int score; public Student(int score) {...} }
// C++： 访问修饰符后面要写冒号，用 `private:` / `public:` 分段，
//       而不是在每个成员前重复写；类定义结尾必须有分号。
// ============================================================================
class Student
{
private:                // C# 是 `private int score;`，C++ 要写成 `private:` 再单独声明
    int score;          // 成员变量（字段）：C++ 习惯加 `_` 或 `m_` 前缀，这里按 C# 风格保留原名

public:                 // C# 在每个成员前写 public，C++ 只需写一次 `public:` 管到下一个修饰符
    // 构造函数：C++ 没有 `this.score` 这种写法
    // 成员初始化列表（: score(score)）是 C++ 推荐做法：在对象创建时直接初始化成员，比在函数体里赋值效率更高
    Student(int score) : score(score)
    {
        // C++ 里 `this` 是「指针」，所以访问成员必须用 `->`：this->score = score;
        // 这里用了上面的初始化列表，函数体可以留空
    }

    // getter：C# 属性 `public int Score { get; }` 在 C++ 中要用普通成员函数模拟
    // 末尾的 const 表示「该函数不会修改对象成员」，C# 里没有对应语法，但它是好习惯
    int getScore() const
    {
        return score;   // 在成员函数内部直接写成员名，等价于 this->score
    }

};                      // 注意：C# 的 class 结尾可以不写分号，C++ 的 class 定义结尾必须写分号

// ============================================================================
// 二、析构函数（Destructor）—— C# 没有对应物
// ----------------------------------------------------------------------------
// C#： 没有析构函数，对象由 GC 在「不确定的时刻」回收，所以不能靠它释放资源（要用 IDisposable / using）。
// C++： 没有 GC！对象离开作用域就立即析构，这是 C++ 资源管理的基石。
//       ~类名() 就是析构函数：无参数、无返回值、不可重载、不能显式调用。
//       ★ 规矩：谁申请，谁释放 —— 在构造函数里申请的资源，必须在析构函数里释放。
// ============================================================================
class FileHolder
{
private:
    char* buffer;           // 模拟「构造函数里申请的动态资源」（堆内存）

public:
    FileHolder(size_t size) : buffer(new char[size])   // 构造：申请资源
    {
        cout << "  [FileHolder 构造] 申请了 " << size << " 字节" << endl;
    }

    // ★ 析构函数：不用手动调用，对象生命周期结束时会自动执行
    ~FileHolder()
    {
        delete[] buffer;    // 释放构造里 new[] 出来的内存
        cout << "  [FileHolder 析构] 释放了内存" << endl;
    }
};

// ============================================================================
// 三、拷贝构造 / 拷贝赋值 / 移动语义 —— C# 完全没有的概念
// ----------------------------------------------------------------------------
// C#： 类对象是「引用类型」，赋值 `b = a` 只是让两个变量指向同一个对象，从不复制对象本身。
// C++： 对象是「值类型」，`B b = a;` 默认会【真的把 a 逐个成员复制成 b】！
//      如果类里持有裸指针，默认的浅拷贝会让两个对象指向同一块内存，
//      然后析构两次 -> 程序崩溃。这就是著名的「三/五法则」。
//      下面用 std::vector 成员演示（vector 自己会深拷贝，所以安全）；
//      但类里只要有裸指针，就必须自己写拷贝构造 / 拷贝赋值 / 析构（三法则）。
// ============================================================================
class Team
{
private:
    string name;
    vector<int> scores;     // vector 是「值语义」容器，拷贝时会自动深拷贝所有元素

public:
    Team(string name, vector<int> scores) : name(move(name)), scores(move(scores)) {}

    // 拷贝构造函数：C# 没有这个语法。
    // 签名固定：类名(const 类名& other)，用别的已有对象来「初始化新对象」时被调用。
    // 这里的 : scores(other.scores) 会调用 vector 的拷贝构造，完成深拷贝。
    Team(const Team& other) : name(other.name + "(副本)"), scores(other.scores)
    {
        cout << "  [Team 拷贝构造] 深拷贝了 " << scores.size() << " 个分数" << endl;
    }

    // 拷贝赋值运算符：`t2 = t1;`（两个都已存在的对象之间赋值）时被调用
    Team& operator=(const Team& other)
    {
        if (this != &other)          // 必须防「自己赋值给自己」
        {
            name = other.name;
            scores = other.scores;   // vector 的赋值运算符自己做深拷贝
        }
        return *this;                // 固定要返回 *this，才能支持链式 `a = b = c;`
    }

    // 移动构造：C# 没有对应物（`move(x)` 也是 C++ 特有的）。
    // C++11 起，`std::move(x)` 表示「我不再用 x 了，把它的内部资源直接搬给新对象」，
    // 参数是 `类名&&`（右值引用）。对 vector 来说，移动只是偷走内部指针，不逐个复制元素，快得多。
    Team(Team&& other) noexcept : name(move(other.name)), scores(move(other.scores))
    {
        cout << "  [Team 移动构造] 直接搬走资源，没有复制元素" << endl;
    }

    size_t count() const { return scores.size(); }
    const string& getName() const { return name; }
};

// ============================================================================
// 四、继承 + 虚函数（多态）—— C# 的 virtual / override / abstract
// ----------------------------------------------------------------------------
// C#： 方法默认不参与多态，想被重写必须写 virtual；子类重写要写 override；
//      抽象方法用 abstract。
// C++： 关键字一样叫 virtual（但【不写 override 也能编译】；override 是 C++11 的可选检查，
//      强烈建议写上，写错了编译器会报错）。
//      纯虚函数 `= 0` 相当于 C# 的 abstract，含纯虚函数的类是抽象类，不能被实例化。
// ============================================================================
class Animal                    // 抽象基类，相当于 C# 的 abstract class
{
public:
    // ★ virtual + = 0 是「纯虚函数」，相当于 C# 的 public abstract void Speak();
    virtual void speak() const = 0;

    // 普通的 virtual 函数，相当于 C# 的 public virtual void Sleep()
    virtual void sleep() const
    {
        cout << "  ...动物在睡觉" << endl;
    }

    // ★ 多态基类必须有虚析构函数，否则用基类指针 delete 时不会调用子类析构函数
    virtual ~Animal() { cout << "  [Animal 析构]" << endl; }
};

class Dog : public Animal       // C# 是 `class Dog : Animal`；C++ 要写清继承方式 `: public`
{
public:
    // override 是为了让编译器帮你检查「确实重写了基类的虚函数」，写错会编译报错
    void speak() const override
    {
        cout << "  汪汪！" << endl;
    }

    ~Dog() override { cout << "  [Dog 析构]" << endl; }
};

class Cat : public Animal
{
public:
    void speak() const override
    {
        cout << "  喵～" << endl;
    }

    ~Cat() override { cout << "  [Cat 析构]" << endl; }
};

// ============================================================================
// 五、智能指针 —— 用 RAII 替代 C# 的 GC
// ----------------------------------------------------------------------------
// C#： 对象 new 出来就不用管了，GC 会在不确定的时刻回收。
// C++： 没有 GC，所以标准库提供了「智能指针」，本质是 RAII 包装：
//      在栈上放一个对象，它析构时自动 delete 它管理的堆内存。
// ★ 现代 C++ 的默认选择：能用 unique_ptr 就用 unique_ptr，
//   只有确实需要「共享所有权」时才用 shared_ptr。
//
//   unique_ptr<T>  —— 独占所有权，不能复制，只能 move。零额外开销。
//   shared_ptr<T>  —— 共享所有权，内部有引用计数，计数归零时自动 delete。
//   weak_ptr<T>    —— 不增加引用计数的「弱引用」，用于打破 shared_ptr 的循环引用
//                     （对应 C# 的 WeakReference）。
// ============================================================================
void demoSmartPointers()
{
    cout << "--- 5.1 unique_ptr：独占所有权（推荐默认）---" << endl;
    // C# 是 `var s = new Student(95);`
    // C++ 是 std::make_unique<Student>(95)，返回 unique_ptr<Student>
    unique_ptr<Student> u = make_unique<Student>(95);
    cout << "  u->getScore() = " << u->getScore() << endl;   // -> 访问成员，和裸指针一样

    // unique_ptr 不能复制：下一行如果打开会编译报错（C# 里 `var u2 = u;` 是可以的）
    // unique_ptr<Student> u2 = u;                  // ❌ 编译错误
    unique_ptr<Student> u3 = move(u);               // ✔ 转移所有权后 u 变成 nullptr
    cout << "  move 之后 u==nullptr ？ " << (u == nullptr ? "是" : "否") << endl;

    cout << "--- 5.2 shared_ptr：共享所有权（引用计数）---" << endl;
    shared_ptr<Student> s1 = make_shared<Student>(60);
    cout << "  创建后 引用计数 = " << s1.use_count() << endl;   // 1
    {
        shared_ptr<Student> s2 = s1;                 // 复制：计数变 2
        cout << "  复制后 引用计数 = " << s1.use_count() << endl; // 2
    }                                                // s2 离开作用域，计数回到 1
    cout << "  离开作用域后 引用计数 = " << s1.use_count() << endl; // 1
    // s1 在函数结束时析构，计数归 0，Student 才真正被 delete

    cout << "--- 5.3 多态 + unique_ptr（向上转型）---" << endl;
    // C#： `Animal a = new Dog();`  C++： unique_ptr<Animal> a = make_unique<Dog>();
    vector<unique_ptr<Animal>> zoo;
    zoo.push_back(make_unique<Dog>());           // 存进去时自动向上转型成 Animal*
    zoo.push_back(make_unique<Cat>());
    for (const auto& animal : zoo)               // C++11 的 range-for，对应 C# 的 foreach
    {
        animal->speak();                         // ★ 多态：靠虚函数表动态决定调用哪个 speak()
    }
    // ★ 关键：这里不用手动 delete。vector 析构时每个 unique_ptr 析构，
    //   又因为 Animal 有虚析构函数，所以 Dog/Cat 的析构函数也会被正确调用。
}

// ============================================================================
// 六、循环引用：为什么需要 weak_ptr（shared_ptr 的经典陷阱）
// ----------------------------------------------------------------------------
// C# 里 A 引用 B、B 又引用 A，GC 能识别出「这俩都没人用了」然后回收（可达性分析）。
// C++ 的 shared_ptr 只靠引用计数，互相持有会让计数永远 >= 1，内存永远不释放 —— 内存泄漏！
// 解决办法：其中一方改用 weak_ptr（不增加计数），需要用时用 .lock() 临时提升成 shared_ptr。
// ============================================================================
class Person;

class Phone
{
public:
    weak_ptr<Person> owner;                     // ★ 用 weak_ptr，避免和 Person 互相持有
    ~Phone() { cout << "  [Phone 析构]" << endl; }
};

class Person
{
public:
    shared_ptr<Phone> phone;                    // Person 强引用 Phone
    ~Person() { cout << "  [Person 析构]" << endl; }
};

void demoCycleBreak()
{
    shared_ptr<Person> p = make_shared<Person>();
    shared_ptr<Phone> ph = make_shared<Phone>();
    p->phone = ph;                              // Person 持有 Phone
    ph->owner = p;                              // Phone「弱」持有 Person，不增加计数

    cout << "  Person 引用计数 = " << p.use_count() << "（应该只有 1）" << endl;
    // 如果把 weak_ptr 换成 shared_ptr，这句会输出 2，两个对象都永远析构不了 -> 泄漏
    // 现在函数结束时计数归 0，两个析构函数都会打印出来
}

// ============================================================================
// main：按编号顺序演示每一节
// ============================================================================
int main()
{
    cout << "===== 1. 基本类：栈上对象（C# 里是堆上 + GC）=====" << endl;
    // C# 是 `Student s = new Student(95);`（堆上，GC 回收）
    // C++ 是 `Student s(95);`（栈上，离开作用域自动析构，没有 GC）
    Student s(95);
    cout << "  score = " << s.getScore() << endl;
    // 因为 getScore() 是 const 函数，常量对象也能调用：
    const Student cs(60);
    cout << "  const 对象的 score = " << cs.getScore() << endl;

    cout << endl << "===== 2. 析构函数：离开作用域自动触发 =====" << endl;
    {
        FileHolder fh(10);          // 构造
        cout << "  对象还活着" << endl;
    }                               // ★ 这里 } 一到，fh 自动析构，不用手动释放
    cout << "  已离开作用域，资源自动释放" << endl;

    cout << endl << "===== 3. 拷贝 / 移动语义 =====" << endl;
    Team t1("红队", {90, 85, 77});
    Team t2 = t1;                   // 触发「拷贝构造」：t2 是独立副本
    cout << "  t2.name = " << t2.getName() << endl;
    Team t3 = move(t1);             // 触发「移动构造」：直接搬走资源
    cout << "  t3.size = " << t3.count() << "，t1 被掏空后 size = " << t1.count() << endl;

    cout << endl << "===== 4. 继承 + 虚函数（多态）=====" << endl;
    Dog d;
    Cat c;
    Animal* pa = &d;                // 用基类指针指向子类对象
    pa->speak();                    // ★ 虚函数生效：调用的是 Dog::speak()
    pa = &c;
    pa->speak();                    // ★ 现在调用 Cat::speak()

    cout << endl << "===== 5. 智能指针 =====" << endl;
    demoSmartPointers();

    cout << endl << "===== 6. weak_ptr 打破循环引用 =====" << endl;
    demoCycleBreak();

    cout << endl << "===== 程序结束（栈上对象逆序析构）=====" << endl;
    return 0;
}
