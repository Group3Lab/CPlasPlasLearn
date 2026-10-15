#include <iostream>
#include <memory>

using namespace std;

class Student
{
private:
    int point_;
public:
    Student(int point) : point_(point)
    {}

    ~Student() 
    {
        cout << "destryed" << endl;
    }

    void printPoint()
    {
        cout << point_<<endl;
    }
};

int main() 
{
    {
    unique_ptr<Student> a = make_unique<Student>(85); //现代写法
    //unique_ptr<Student> a(new Student(85)); //传统写法

    a->printPoint();
    }
    return 0;
}
