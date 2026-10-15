#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Animal
{
public:
    virtual void speak() const = 0;

protected:
    Animal(string n, string t) : name(move(n)), type(move(t)) {}

    string name;
    string type;
};

class Dog : public Animal
{
public:
    Dog(string name) : Animal(move(name), "Dog") {}

    void speak() const override
    {
        cout << name << " says: wowo" << endl;
    }
};

int main()
{
    unique_ptr<Animal> a = make_unique<Dog>("Buddy");

    a->speak();

    return 0;
}
