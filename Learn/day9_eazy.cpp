// ============================================================================
// day9_eazy：C# 风格的 C++ OOP —— 只用「舒服的那一列」
// ----------------------------------------------------------------------------
// 这个文件的目的：给你一套可以无脑照抄的默认写法。
// 凡是 C++ 里有多种写法的，这里只用现代、最接近 C# 的那一种。
// 老写法（裸 new/delete、char*、typedef、NULL…）这里故意不出现。
//
// 本文件的四条硬规矩：
//   1. 对象一律用智能指针创建：默认 make_unique，确实要共享才 make_shared。
//   2. 绝手写 delete —— 一个都不写，全靠 RAII 自动回收（相当于 C# 的 GC）。
//   3. 成员函数只要不改状态就加 const，当默认写。
//   4. 继承用 : public，重写虚函数一律写 override。
// ============================================================================
#include <iostream>
#include <string>
#include <vector>
#include <memory>       // unique_ptr / shared_ptr / weak_ptr

using namespace std;

// ============================================================================
// 一、基类：像 C# 的 abstract class
// ----------------------------------------------------------------------------
// 对应 C#：
//   abstract class Animal
//   {
//       public abstract string Speak();          // 抽象方法
//       public virtual void Sleep() { ... }      // 虚方法
//   }
// ============================================================================
class Animal
{
public:
    // C++ 的抽象方法 = 「纯虚函数」，写法是 = 0
    virtual string speak() const = 0;

    // 普通虚方法：子类可重写也可不重写
    virtual void sleep() const
    {
        cout << "  " << name_ << " 睡觉中..." << endl;
    }

    // 只读属性：C# 的 public string Name { get; }
    // 返回 const 引用避免复制；函数末尾的 const 表示不修改对象
    const string& name() const { return name_; }

    // ★ 虚析构函数：多态基类必须写。
    //   不写的话，用基类智能指针析构子类对象时，子类析构函数不会被调用。
    //   这是 C++ 相对 C# 最容易踩的坑之一（C# 不需要关心这个）。
    virtual ~Animal() = default;

protected:
    // protected 构造：让这个类只能被继承，不能被直接创建（和 C# 的抽象类效果一致）
    explicit Animal(string name) : name_(move(name)) {}

private:
    string name_;
};

// ============================================================================
// 二、子类：像 C# 的普通继承
// ----------------------------------------------------------------------------
// 对应 C#：  class Dog : Animal { public override string Speak() => "汪汪！"; }
// ============================================================================
class Dog : public Animal        // C# 写 `: Animal`；C++ 要写清继承方式 `: public`
{
public:
    Dog() : Animal("小狗") {}

    // override 让编译器检查「确实重写了基类虚函数」，写错立刻报错
    string speak() const override { return "汪汪！"; }
};

class Cat : public Animal
{
public:
    Cat() : Animal("小猫") {}
    string speak() const override { return "喵～"; }
};

// ============================================================================
// 三、带资源的类：构造函数申请、析构函数自动释放
// ----------------------------------------------------------------------------
// 对应 C#： 这是需要 IDisposable + using 才能做的事。
// C++ 里只要写对析构函数，离开作用域就自动释放，不用 using，也不会忘。
// ★ 这就是 RAII：资源获取即初始化，对象的生命周期 = 资源的生命周期。
// ============================================================================
class Logger
{
public:
    explicit Logger(string tag) : tag_(move(tag))
    {
        cout << "  [Logger " << tag_ << " 打开]" << endl;
    }

    ~Logger()
    {
        cout << "  [Logger " << tag_ << " 关闭]  ← 自动触发，不用手写" << endl;
    }

    void log(const string& msg) const
    {
        cout << "  [" << tag_ << "] " << msg << endl;
    }

private:
    string tag_;
};

// ============================================================================
// 四、拷贝 / 移动：C# 没有，但现代 C++ 的正确做法很省事
// ----------------------------------------------------------------------------
// C# 里对象是引用类型，赋值不复制对象，所以没有这些概念。
// C++ 里对象是值类型，默认会真的复制。但【只要成员都是智能指针/string/vector
// 这类自己管好资源的类型，你什么都不用写，编译器自动生成的就是对的！】
//
// ★ 本文件的关键简化：成员一律用智能指针和标准容器，就永远不用手写
//   拷贝构造 / 拷贝赋值 / 移动构造 / 析构 —— 这就是「零法则（Rule of Zero）」。
//   反面教材：成员里一旦有裸指针，就必须手写这些，否则会 double free 崩溃。
// ============================================================================
class School
{
public:
    School(string name) : name_(move(name)) {}
    // 注意这里：没有析构函数、没有拷贝构造、没有移动构造 —— 一行都不用写！

    void addStudent(unique_ptr<Animal> a)
    {
        // move 转移所有权；成员是 vector<unique_ptr>，容器自己会管好释放
        students_.push_back(move(a));
    }

    void rollCall() const
    {
        cout << "  " << name_ << " 点名：" << endl;

        // ★ range-for 是 C++ 最像 C# foreach 的写法，强烈推荐：
        //      C#： foreach (Animal s in students)      { ... }
        //      C++： for (const Animal& s : students)   { ... }
        //   读作「对 students 里的每一个元素 s」。自动从头走到尾，不用管下标、不用管长度。
        //
        //   ★ 关键：students_ 的元素类型是 unique_ptr<Animal>（智能指针），不是 Animal！
        //     所以遍历变量的类型要写成「指向元素的 const 引用」：const unique_ptr<Animal>&
        //     再用 s->name() 访问（-> 是「透过智能指针访问对象」）。
        //
        //   为什么用 const 引用（const unique_ptr<Animal>&）而不是值：
        //     加 & 表示不复制这个智能指针。★ unique_ptr 根本不能复制，写值会直接编译报错。
        //     加 const 表示循环里不会修改它（这里是只读点名，所以加上）。
        //   ★ 如果哪天要修改元素本身，才用非 const 引用：unique_ptr<Animal>&
        for (const unique_ptr<Animal>& s : students_)
        {
            cout << "    " << s->name() << " 说 " << s->speak() << endl;
            // s 是智能指针，所以用 -> 访问；如果写成 (*s).name() 也等价，只是不好看
        }

        // 变体：如果容器里存的是【对象本身】（vector<Animal>，不是指针），那么遍历变量就是
        //      for (const Animal& s : animals)  { cout << s.name(); }   ← 用 . 而不是 ->
        //   「容器元素类型是什么，遍历变量就写什么」，这一条记住就不会错。

        // 对照：如果硬要用下标写（更接近 C# 的 for，但啰嗦且容易越界），是这样的：
        //   for (size_t i = 0; i < students_.size(); i++)
        //   {
        //       const unique_ptr<Animal>& s = students_[i];
        //       cout << "    " << s->name() << " 说 " << s->speak() << endl;
        //   }
        // 能看出 range-for 省掉了「下标、长度」两件容易出错的事。
    }

    size_t count() const { return students_.size(); }

private:
    string name_;                                // std::string：对应 C# string
    vector<unique_ptr<Animal>> students_;        // vector<unique_ptr>：对应 C# List<Animal>
};

// ============================================================================
// 五、共享所有权：什么时候真的需要 shared_ptr
// ----------------------------------------------------------------------------
// 默认用 unique_ptr（独占，零开销）。只有下面这种情况才用 shared_ptr：
// 【多个对象/多个地方，都需要长期持有同一个对象，且谁都不确定谁先死。】
//
// 比如一个「配置对象」，多个服务都要用它，且生命周期不归某一个服务所有。
// ============================================================================
class Config
{
public:
    explicit Config(string path) : path_(move(path))
    {
        cout << "  读取配置：" << path_ << endl;
    }
    ~Config() { cout << "  配置释放（所有使用者都放手了）" << endl; }

    const string& path() const { return path_; }

private:
    string path_;
};

class Service
{
public:
    // 构造函数接收 shared_ptr 并【按值复制】：让引用计数 +1，服务全程持有它
    Service(string name, shared_ptr<Config> cfg)
        : name_(move(name)), cfg_(move(cfg)) {}

    void run() const
    {
        cout << "  服务 " << name_ << " 使用配置 " << cfg_->path()
             << "（当前使用者 " << cfg_.use_count() << " 个）" << endl;
    }

private:
    string name_;
    shared_ptr<Config> cfg_;     // ★ 共享持有：这里用 shared_ptr 是对的
};

// ============================================================================
// 六、循环引用：全用 shared_ptr 的最大陷阱
// ----------------------------------------------------------------------------
// C# 有 GC，A 和 B 互相引用也不会泄漏（可达性分析能识别出没人用了）。
// C++ 的 shared_ptr 靠引用计数，互相持有会让计数永远 >= 1，【内存永远不释放】。
// 解法：其中一方改成 weak_ptr（弱引用，不增加计数）。
//
// ★ 规律：父子/从属关系里，「子」持有「父」就用 weak_ptr，避免子拖住父。
// ============================================================================
class Child;    // 前向声明：Parent 里要用到 Child，但 Child 的完整定义在后面

class Parent : public enable_shared_from_this<Parent>   // ★ 想拿到自身的 shared_ptr 就要继承它
{
public:
    void addChild(shared_ptr<Child> c);     // 定义放到 Child 完整定义之后（见下方）
    size_t childCount() const { return children_.size(); }
    ~Parent() { cout << "  [Parent 析构]" << endl; }

private:
    vector<shared_ptr<Child>> children_;
};

class Child
{
public:
    // ★ 反向引用用 weak_ptr：不增加父对象的计数，避免互相拖住
    void setParent(const shared_ptr<Parent>& p) { parent_ = p; }   // weak_ptr 可由 shared_ptr 赋值
    ~Child() { cout << "  [Child 析构]" << endl; }

private:
    weak_ptr<Parent> parent_;
};

// ★ 类外定义：因为这里要用到 Child 的完整定义（c->setParent），
//   所以不能写在类内部 —— 这是 C++ 的「前向声明 + 类外定义」惯用法。
inline void Parent::addChild(shared_ptr<Child> c)
{
    c->setParent(shared_from_this());   // 安全地把「自己」交给子对象（由 weak_ptr 接收）
    children_.push_back(move(c));       // 正向引用用 shared_ptr：父「拥有」子
}

// ============================================================================
// main：演示每一节
// ============================================================================
int main()
{
    cout << "===== 1. 智能指针的多态：相当于 C# 的 Animal a = new Dog(); =====" << endl;
    // C#： Animal a = new Dog();            a.Speak();
    // C++： unique_ptr<Dog> a = make_unique<Dog>();  a->speak();
    unique_ptr<Dog> a = make_unique<Dog>();
    cout << "  " << a->name() << " 说 " << a->speak() << endl;
    // 用完自动回收，没有 delete，没有 GC。

    cout << endl << "===== 2. unique_ptr 不能复制，只能 move（对照 C# 引用赋值）=====" << endl;
    unique_ptr<Cat> b = make_unique<Cat>();
    // unique_ptr<Cat> c2 = b;     // ❌ 编译错误：unique_ptr 独占，不能复制
    //   C# 里 `var c = b;` 是合法的，两个变量指向同一对象；C++ 里不允许，必须明确转移。
    unique_ptr<Cat> c = move(b);   // ✔ 明确表示「所有权转交给 c」
    cout << "  move 之后 b == nullptr ? " << (b == nullptr ? "是" : "否") << endl;
    cout << "  c 还能用：" << c->speak() << endl;

    cout << endl << "===== 3. RAII：构造申请、析构自动释放（相当于 using 但不用手写）=====" << endl;
    {
        unique_ptr<Logger> lg = make_unique<Logger>("app");
        lg->log("这条日志写在作用域内");
    }                              // ★ 这里自动析构，等价于 C# 的 using 块结束
    cout << "  已离开作用域" << endl;

    cout << endl << "===== 4. 容器 + 多态 + 零法则：一个类管一堆对象 =====" << endl;
    unique_ptr<School> school = make_unique<School>("第一小学");
    school->addStudent(make_unique<Dog>());
    school->addStudent(make_unique<Cat>());
    school->addStudent(make_unique<Dog>());
    school->rollCall();
    cout << "  共 " << school->count() << " 名学生（School 类里没有写任何析构函数）" << endl;

    cout << endl << "===== 5. shared_ptr：真正需要「共享所有权」时才用 =====" << endl;
    {
        shared_ptr<Config> cfg = make_shared<Config>("app.yaml");   // 创建共享对象
        cout << "  创建后使用者 = " << cfg.use_count() << endl;
        unique_ptr<Service> s1 = make_unique<Service>("认证服务", cfg);  // 复制 shared_ptr，计数 +1
        unique_ptr<Service> s2 = make_unique<Service>("订单服务", cfg);  // 再 +1
        cout << "  两个服务都拿到后使用者 = " << cfg.use_count() << endl;
        s1->run();
        s2->run();
    }   // 作用域结束：s1、s2、cfg 依次释放，计数归 0 时才真正 delete
    cout << "  全部释放完毕" << endl;

    cout << endl << "===== 6. weak_ptr 打破循环引用 =====" << endl;
    {
        shared_ptr<Parent> parent = make_shared<Parent>();
        parent->addChild(make_shared<Child>());
        parent->addChild(make_shared<Child>());
        cout << "  父对象有 " << parent->childCount() << " 个子对象" << endl;
        cout << "  父对象的引用计数 = " << parent.use_count() << "（子对象的 weak_ptr 没算进去）" << endl;
        // 如果 Child::parent_ 写成 shared_ptr，这里计数会变成 3，
        // 于是谁都不会先放手 -> 两个对象永远泄漏，析构函数永远不打印。
    }
    cout << "  父子都正常释放（上面打印了析构）" << endl;

    cout << endl << "===== 程序结束（所有智能指针自动回收）=====" << endl;
    return 0;
}
