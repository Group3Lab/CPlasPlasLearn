// ============================================================================
// day9_members：C# 类里的各种成员 → C++ 对应写法速查
// ----------------------------------------------------------------------------
// 这个文件专门解决一个问题：
//   「我在 C# 里习惯这么写，到 C++ 里该写成什么？」
//
// 下面按 C# 类里最常用的成员类型逐条列出，每条都给出：
//   C# 写法   →   C++ 写法   →   注意事项
//
// ★ 配套文件：
//   day9_oop.cpp   —— 完整图鉴（C++ 有什么）
//   day9_eazy.cpp  —— 现代写法模板（默认用智能指针 + 零法则）
//   day9_members.cpp（本文件）—— C# 成员 → C++ 成员 逐条对照
//
// 本文件规矩：不用 auto，类型全部写清楚，方便和 C# 的显式类型对照。
// ============================================================================
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// ============================================================================
// 【1】字段 / 属性 / 常量
// ----------------------------------------------------------------------------
// C#：
//   private int score;                                  // 字段
//   public int Score { get; set; }                       // 自动属性
//   public int Score2 { get; private set; }              // 只读外部、内部可写
//   public int Score3 { get; } = 100;                    // 只读属性 + 初始值
//   public const int MaxScore = 100;                     // 编译期常量
//   public static readonly int Limit = 60;               // 运行期只读静态
//
// C++：
//   ★ C++ 没有「属性（property）」这个语法！只能靠成员函数模拟。
//     这是从 C# 过来最大的不适应点。
// ============================================================================
class Score
{
private:
    int score_ = 0;                 // 字段：可以就地给默认值（对应 C# 的字段初始化器）
    static int instanceCount_;      // 静态成员：声明放在类里，定义必须在类外（见文件末尾）

public:
    // --- 对应 C# 的 public int Score { get; set; } ---
    // getter + setter 两个函数配合，就是 C# 的读写属性
    int getScore() const { return score_; }        // 只读访问：const 表示不改对象
    void setScore(int value) { score_ = value; }   // 写入访问

    // --- 对应 C# 的 public int Score3 { get; } = 100; （只读属性）---
    // C++ 里就是「只有 getter，没有 setter」，靠 getter 的 const 保证只读
    static int getMaxScore() { return 100; }       // 静态方法：对应 C# 的 static 方法

    // --- 对应 C# 的 public const int MaxScore = 100; ---
    // C++ 用 static const（类内整数常量）或 static constexpr（推荐，C++11 起）
    static const int MAX_SCORE = 100;
    static constexpr double PI = 3.14159;          // constexpr 更好：编译期就求值

    // --- 对应 C# 的构造函数 ---
    Score() { ++instanceCount_; }
    explicit Score(int initial) : score_(initial) { ++instanceCount_; }   // explicit 防隐式转换

    // --- 对应 C# 的静态构造函数 static Score() {} ---
    // C++ 没有「静态构造函数」这个语法！静态成员的初始化在类外进行（见文件末尾）。
    static int getInstanceCount() { return instanceCount_; }
};

// ★ 静态成员的「定义」必须写在类外（这一步相当于 C# 的 static 字段）
//   C++17 起也可以写成 inline static int instanceCount_ = 0; 放在类内，
//   但为了和 C# 的「声明/初始化分开」概念对应，这里用传统写法。
int Score::instanceCount_ = 0;

// ============================================================================
// 【2】构造函数：默认 / 带参 / 重载 / 委托构造
// ----------------------------------------------------------------------------
// C#：
//   public Person() { Name = "无名"; }                      // 无参构造
//   public Person(string name) { Name = name; }             // 带参构造
//   public Person(string name, int age) : this(name) { ... } // 委托构造（this(...)）
//
// C++：
//   构造函数可以重载（同名不同参数），和 C# 一样。
//   委托构造用「构造函数初始化列表里的类名(...)」表示。
//   ★ C++11 起可以用 = default 让编译器生成默认实现。
// ============================================================================
class Person
{
private:
    string name_;
    int age_;

public:
    // 对应 C#： public Person() { Name = "无名"; }
    Person() : age_(0)                       // 无参构造：name_ 默认就是空串
    {
        name_ = "无名";
    }

    // 对应 C#： public Person(string name) { ... }
    explicit Person(string name) : name_(move(name)), age_(0) {}

    // 对应 C#： public Person(string name, int age) : this(name) { this.Age = age; }
    // C++ 的「委托构造」：在初始化列表里调用同类的另一个构造函数
    Person(string name, int age) : Person(move(name))
    {
        age_ = age;
    }

    // 对应 C# 编译器自动生成的默认构造：C++ 也可以让编译器生成
    // Person() = default;                  // 如果没写别的构造，这行才可用

    const string& getName() const { return name_; }
    int getAge() const { return age_; }
};

// ============================================================================
// 【3】方法：普通 / 静态 / 只读（C# 没有）/ 重载
// ----------------------------------------------------------------------------
// C#：
//   public int Add(int a, int b) => a + b;            // 实例方法
//   public static int Mul(int a, int b) => a * b;     // 静态方法
//   public int Add(int a, int b, int c) => ...;       // 重载
//   （C# 没有办法标记「此方法不改状态」）
//
// C++：
//   普通方法和 C# 几乎一样，只是**必须声明返回类型在前**（C# 是 `返回值 名字`，其实一样）。
//   ★ C++ 独有：末尾加 const 表示「该方法不修改对象」，是 const 正确性的核心。
// ============================================================================
class Calculator
{
public:
    // 实例方法：和 C# 写法基本一致
    int add(int a, int b) const { return a + b; }        // ★ const = 不改对象

    // 重载：同名、参数不同 —— 和 C# 完全一样
    int add(int a, int b, int c) const { return a + b + c; }

    // 静态方法：用 static 关键字，和 C# 一样
    static int multiply(int a, int b) { return a * b; }
};

// ============================================================================
// 【4】继承：构造 / base 调用 / 重写 / 抽象 / 密封
// ----------------------------------------------------------------------------
// C#：
//   public abstract class Animal
//   {
//       protected string Name;
//       public Animal(string name) { Name = name; }
//       public abstract string Speak();               // 抽象方法
//       public virtual void Sleep() { ... }           // 虚方法
//   }
//   public class Dog : Animal
//   {
//       public Dog() : base("狗") { }                 // 调用基类构造
//       public override string Speak() => "汪汪！";    // 重写
//       public override sealed void Sleep() { }       // 密封重写
//   }
//
// C++：
//   ★ 继承必须写明继承方式 `: public`（C# 里只写 `: Animal`）。
//   ★ 调用基类构造用初始化列表里的「基类名(...)」，对应 C# 的 `: base(...)`。
//   ★ 重写用 override（C++11 起可用，强烈建议写）。
//   ★ 密封：C# 的 sealed → C++ 的 final。
// ============================================================================
class Animal
{
public:
    explicit Animal(string name) : name_(move(name)) {}   // 对应 C# public Animal(string name)

    // 对应 C# 的 public abstract string Speak();
    // C++ 的「纯虚函数」写法 = 0，含它的类是抽象类，不能实例化
    virtual string speak() const = 0;

    // 对应 C# 的 public virtual void Sleep()
    virtual void sleep() const
    {
        cout << "  " << name_ << " 睡觉中..." << endl;
    }

    const string& name() const { return name_; }

    // 多态基类必须有虚析构函数
    virtual ~Animal() = default;

protected:                              // 对应 C# 的 protected：只有子类能访问
    string name_;
};

class Dog : public Animal               // ★ 必须写 `public`，否则变成私有继承（拿不到 public 成员）
{
public:
    // 对应 C# 的 public Dog() : base("狗") { }
    // C++ 写法：在初始化列表里直接写基类名
    Dog() : Animal("狗") {}

    // 对应 C# 的 public override string Speak()
    string speak() const override { return "汪汪！"; }

    // 对应 C# 的 public override sealed void Sleep()
    // C++ 的 final = C# 的 sealed：子类不能再重写
    void sleep() const final
    {
        cout << "  狗趴着睡" << endl;
    }
};

// ============================================================================
// 【5】接口 / 抽象：C# 的 interface → C++ 的「纯抽象类」
// ----------------------------------------------------------------------------
// C#：
//   public interface IFlyable { void Fly(); int MaxSpeed { get; } }
//   public interface ISwimmable { void Swim(); }
//   public class Duck : Animal, IFlyable, ISwimmable { ... }   // 可多继承接口
//
// C++：
//   ★ C++ 没有 interface 关键字，用「全是纯虚函数的类」+ 公开继承来模拟。
//   ★ 命名惯例：C# 用 I 前缀（IFlyable），C++ 常用能/行为后缀或直接名词。
//   ★ ★ C++ 允许多继承（这是 C# 没有的），所以能实现多个接口。
//       但多继承要小心「菱形继承」，一般只用来继承「纯接口」。
// ============================================================================
class Flyable
{
public:
    virtual void fly() const = 0;
    virtual int maxSpeed() const = 0;
    virtual ~Flyable() = default;       // 接口的析构也要虚
};

class Swimmable
{
public:
    virtual void swim() const = 0;
    virtual ~Swimmable() = default;
};

// ★ 同时继承一个基类 + 两个接口，对应 C# 的 `class Duck : Animal, IFlyable, ISwimmable`
class Duck : public Animal, public Flyable, public Swimmable
{
public:
    Duck() : Animal("鸭子") {}

    string speak() const override { return "嘎嘎！"; }

    void fly() const override { cout << "  鸭子飞不起来，只能扑腾" << endl; }
    int maxSpeed() const override { return 5; }
    void swim() const override { cout << "  鸭子游泳中" << endl; }
};

// ============================================================================
// 【6】运算符重载
// ----------------------------------------------------------------------------
// C#：
//   public static Vector operator +(Vector a, Vector b) => new Vector(a.X + b.X, a.Y + b.Y);
//   public static bool operator ==(Vector a, Vector b) => ...;
//   public override string ToString() => $"({X}, {Y})";
//
// C++：
//   ★ C# 的运算符重载必须是 static，参数要写全两个操作数；
//     C++ 可以写成「成员函数」（只用写右边那个操作数，左边就是 this）。
//   ★ C# 的 ToString() 在 C++ 里是重载 operator<<（配合 ostream 输出）。
// ============================================================================
class Vector2
{
private:
    double x_;
    double y_;

public:
    Vector2(double x, double y) : x_(x), y_(y) {}

    // 成员形式的 + ：对应 C# 的 `public static Vector operator +(Vector a, Vector b)`
    // 但 C++ 只需写右侧操作数，左边隐含是 *this
    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x_ + other.x_, y_ + other.y_);
    }

    // 对应 C# 的 `public static bool operator ==(Vector a, Vector b)`
    bool operator==(const Vector2& other) const
    {
        return x_ == other.x_ && y_ == other.y_;
    }

    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);        // 复用 ==，避免重复逻辑
    }

    double getX() const { return x_; }
    double getY() const { return y_; }
};

// 对应 C# 的 `public override string ToString()`
// C++ 的惯用做法是重载 operator<<，这样 `cout << v` 就能用
ostream& operator<<(ostream& os, const Vector2& v)
{
    os << "(" << v.getX() << ", " << v.getY() << ")";
    return os;                            // 固定要返回 os，才能支持链式 `cout << a << b`
}

// ============================================================================
// 【7】C# 有、C++ 没有的类成员（这些要特别小心）
// ----------------------------------------------------------------------------
//   C# 属性        public int X { get; set; }        → C++ 没有，用 get/set 函数模拟
//   C# 事件        public event EventHandler E;       → C++ 没有，用 std::function / 回调模拟
//   C# 索引器      public int this[int i] { ... }     → C++ 重载 operator[]
//   C# 委托        public delegate void D(int x);     → C++ 用 std::function 或函数指针
//   C# 特性        [Serializable]                     → C++ 没有（C++11 的 attribute 语法不同）
//   C# 分部类      partial class Foo { }              → C++ 没有
//   C# using 别名  using MyList = List<int>;          → C++ 的 using MyList = vector<int>;
//
//   反过来，C++ 有、C# 没有的：
//   const 成员函数  ← C# 无
//   头文件/前向声明  ← C# 无
//   多重继承        ← C# 无（C# 只能多继承接口）
//   模板            ← C# 的泛型类似但机制不同（模板是编译期展开）
// ============================================================================

// 对应 C# 的 `public int this[int i] { get { return items[i]; } }`
class IntList
{
private:
    vector<int> items_;

public:
    void add(int v) { items_.push_back(v); }

    // 对应 C# 的索引器 public int this[int i]
    // C++ 写成 operator[]
    int operator[](size_t index) const { return items_[index]; }

    size_t size() const { return items_.size(); }
};

// ============================================================================
// 【8】速查总表：C# → C++ 类成员对照
// ----------------------------------------------------------------------------
//   C# 写法                                  C++ 写法
//   --------------------------------------   ---------------------------------
//   private int x;                           private: int x;
//   public int X { get; set; }               int getX() const; void setX(int);
//   public int X { get; } = 100;             int x_ = 100; int getX() const;
//   public const int N = 100;                static const int N = 100;  (或 constexpr)
//   public static int N = 5;                 static int N;  + 类外定义
//   static Foo() { }                         ❌ 没有，改为类外初始化静态成员
//   public Foo() { }                         Foo() { }
//   public Foo(int a) { }                    Foo(int a) { }
//   public Foo(int a) : this(1) { }          Foo(int a) : Foo(1) { }
//   public override string ToString()        ostream& operator<<(ostream&, const T&)
//   public int this[int i] { get; }          int operator[](size_t i) const;
//   public static V operator +(V a, V b)     V operator+(const V& o) const;   (成员形式)
//   public virtual void M() { }              virtual void M() { }
//   public abstract void M();                virtual void M() = 0;
//   public override void M() { }             void M() override { }
//   public override sealed void M() { }      void M() final { }
//   public class D : B                       class D : public B
//   public D() : base(x) { }                 D() : B(x) { }
//   public interface IFoo { void M(); }      class Foo { virtual void M() = 0; };  class X : public Foo
//   public class X : B, IFoo, IBar           class X : public B, public Foo, public Bar
//   public event EventHandler E;             ❌ 没有，用 std::function 或回调
//   public delegate void D(int);             ❌ 没有，用 std::function<void(int)>
//   [Serializable]                           ❌ 没有对应物
//   partial class Foo                        ❌ 没有对应物
//   using MyList = List<int>;                using MyList = vector<int>;
//
//   ★ C# 有、C++ 完全没有的：属性、事件、委托、特性、分部类、静态构造函数
//   ★ C++ 有、C# 没有的：const 成员函数、头文件/前向声明、多重继承、模板元编程、
//                        析构函数、拷贝/移动语义、RAII
// ============================================================================

// ============================================================================
// main：把上面每条都跑一遍
// ============================================================================
int main()
{
    cout << "===== 1. 字段 / 属性 / 常量 =====" << endl;
    Score sc;                                  // 用默认构造
    Score sc2(88);                             // 用带参构造（explicit 后不能写 Score sc2 = 88;）
    sc.setScore(75);                           // 对应 C# 的 `sc.Score = 75;`
    cout << "  sc.Score   = " << sc.getScore() << endl;    // 对应 C# 的 `sc.Score`
    cout << "  sc2.Score  = " << sc2.getScore() << endl;
    cout << "  MAX_SCORE  = " << Score::MAX_SCORE << endl;         // 对应 C# 的 Score.MaxScore
    cout << "  PI         = " << Score::PI << endl;
    cout << "  实例个数   = " << Score::getInstanceCount() << endl; // 对应 C# 的 Score.InstanceCount

    cout << endl << "===== 2. 构造函数：无参 / 带参 / 委托构造 =====" << endl;
    Person p1;                                 // 无参构造
    Person p2("小明");                          // 带参构造
    Person p3("小红", 20);                      // 委托构造（内部先调 Person(string)）
    cout << "  p1 = " << p1.getName() << ", " << p1.getAge() << endl;
    cout << "  p2 = " << p2.getName() << ", " << p2.getAge() << endl;
    cout << "  p3 = " << p3.getName() << ", " << p3.getAge() << endl;

    cout << endl << "===== 3. 方法：实例 / 静态 / 重载 / const =====" << endl;
    Calculator calc;
    cout << "  calc.add(2, 3)       = " << calc.add(2, 3) << "   ← 实例方法" << endl;
    cout << "  calc.add(2, 3, 4)    = " << calc.add(2, 3, 4) << "   ← 重载" << endl;
    cout << "  Calculator.multiply  = " << Calculator::multiply(3, 4) << "   ← 静态方法" << endl;

    cout << endl << "===== 4. 继承 + 抽象 + 重写 + final(sealed) =====" << endl;
    // 对应 C# 的 `Animal a = new Dog();`，C++ 用智能指针（见 day9_eazy.cpp 详解）
    unique_ptr<Animal> animal = make_unique<Dog>();
    cout << "  " << animal->name() << " 说 " << animal->speak() << endl;  // 多态
    animal->sleep();                                                      // 调用 final 重写版

    cout << endl << "===== 5. 接口（纯抽象类）+ 多重继承 =====" << endl;
    Duck duck;
    duck.speak();
    duck.fly();
    duck.swim();
    cout << "  鸭子最大速度 = " << duck.maxSpeed() << endl;

    // 也可以只当成 Flyable 用（对应 C# 的 `IFlyable f = duck;`）
    Flyable& f = duck;
    f.fly();                                   // 通过接口引用调用

    cout << endl << "===== 6. 运算符重载 + ToString(operator<<) =====" << endl;
    Vector2 v1(1, 2);
    Vector2 v2(3, 4);
    Vector2 v3 = v1 + v2;                      // 对应 C# 的 `var v3 = v1 + v2;`
    cout << "  v1 + v2 = " << v3 << endl;       // cout << 对应 C# 的 Console.WriteLine(v3)
    cout << "  v1 == v2 ? " << (v1 == v2 ? "true" : "false") << endl;
    cout << "  v1 != v2 ? " << (v1 != v2 ? "true" : "false") << endl;

    cout << endl << "===== 7. 索引器 this[int i] → operator[] =====" << endl;
    IntList list;
    list.add(10);
    list.add(20);
    list.add(30);
    // 对应 C# 的 `list[1]`
    for (size_t i = 0; i < list.size(); ++i)
    {
        cout << "  list[" << i << "] = " << list[i] << endl;
    }

    cout << endl << "===== 演示结束 =====" << endl;
    return 0;
}
